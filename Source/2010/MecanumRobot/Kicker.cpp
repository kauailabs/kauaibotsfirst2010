#include "Kicker.h"

static SEM_ID cKickerStateSemaphore;

/**
 * Internal task.
 * 
 * Task which checks the compressor pressure switch and operates the relay as necessary
 * depending on the pressure.
 * 
 * Do not call this function directly.
 */
static void kickerTask(Kicker *kicker)
{
	while (1)
	{
		if (kicker->GetAutoLoad())
		{
			Kicker::KickerState state = kicker->GetKickerState();
			switch (state)
			{
			case Kicker::Loading:
				// Stay in loading until IsWinchLoaded(), then
				// transition to Loaded
				if ( kicker->GetKickerMotorState() != Kicker::Forward )
				{
					kicker->SetKickerMotorState(Kicker::Forward);
				}
				if ( kicker->IsWinchLoaded() )
				{
					kicker->SetKickerMotorState(Kicker::Off);
					kicker->SetKickerState(Kicker::Loaded);
					kicker->ClearFireRequest();
				}
				else
				{
					Wait(0.005); // TEST this, it may need to be tuned.
				}
				break;
			case Kicker::Loaded:
				// Stay in Loaded until we get the fire command
				// or the end of the game command.
				// At that point, advance motor (.1 seconds?)
				// and transition to unloaded.
				if ( kicker->IsFireRequested() ||
					 ( kicker->GetAutoFire() && kicker->IsBallDetected() ) ||  
				     kicker->IsQuitRequested() )
				{
					// start the motor
					kicker->SetKickerMotorState(Kicker::Forward);
					// Wait for a little bit, to make sure the firing occurs.
					Wait(0.2); // TODO:  Tune this...
					// stop the motor.
					kicker->SetKickerMotorState(Kicker::Off);
					kicker->SetKickerState(Kicker::Unloaded);
				}
				break;
			case Kicker::Unloaded:
				// Wait until we get the fire complete, then
				// transition to the loading state, unless we are
				// at the end of the game.
				if ( !kicker->IsQuitRequested())
				{
					// start the motor
					//if( kicker->IsFireComplete() )
					Wait(0.2); // Instead of waiting for the fire complete, simply wait 200ms.
					{
						kicker->SetKickerState(Kicker::Loading);											
					}
				}
				break;
			}
		}
		else
		{
			Wait(0.5);
		}
	}
}


Kicker::Kicker( UINT32 iJaguarPort, 
				UINT32 iAnalogFireCompleteDetectorPort, 
				UINT32 iAnalogBallDetectorPort, 
				UINT32 iDigitalWinchLoadedDetectorPort,
				UINT32 iFireCompleteRelayPort,
				UINT32 iBallDetectorRelayPort)
			: m_WinchControl(iJaguarPort),			
			m_FireCompleteDetector(iAnalogFireCompleteDetectorPort),
			m_FireCompleteDetectorChannel(iAnalogFireCompleteDetectorPort),
			m_BallDetector(iAnalogBallDetectorPort),
			m_BallDetectorChannel(iAnalogBallDetectorPort),
			m_WinchLoaded(iDigitalWinchLoadedDetectorPort),
			m_FireCompleteDetectorRelay(iFireCompleteRelayPort),
			m_BallDetectorRelay(iBallDetectorRelayPort),
			m_task ("Kicker", (FUNCPTR)kickerTask)
{
	m_FireCompleteDetectorRelay.SetDirection(Relay::kForwardOnly);
	m_FireCompleteDetectorRelay.Set(Relay::kForward);
	m_BallDetectorRelay.SetDirection(Relay::kForwardOnly);
	m_BallDetectorRelay.Set(Relay::kForward);
	m_bAutoLoad = true;
	m_bAutoFire = false;
	m_KickerState = Kicker::Unloaded;	
	m_bFireRequested = false;
	m_bQuitRequested = false;
	// Start the Kicker Task
	
	if (!m_task.Start((INT32)this))
	{
		wpi_fatal(CompressorTaskError);
	}
}

Kicker::~Kicker()
{
	// Shut down the Kicker task
}

void Kicker::GetStatus( float *fireCompleteDetectorValue, 
					bool *fireCompleteDetected,
					float *ballDetectorValue,
					bool *ballDetected,
					bool *winchLoaded,
					KickerMotorState *state,
					bool *autoLoad,
					bool *autoFire)
{
	*fireCompleteDetectorValue = m_FireCompleteDetectorChannel.GetAverageVoltage();
	*fireCompleteDetected = IsFireComplete();
	*ballDetectorValue = m_BallDetectorChannel.GetAverageVoltage();
	*ballDetected = IsBallDetected();
	*winchLoaded = IsWinchLoaded();
	*state = GetKickerMotorState();
    *autoLoad = GetAutoLoad();
    *autoFire = GetAutoFire();
}

void Kicker::SetKickerMotorState( KickerMotorState state )
{
	switch(state)
	{
	case Forward:
		m_WinchControl.Set(1);
		break;
	case Reverse:
		m_WinchControl.Set(-1);
		break;
	case Off:
	default:
		m_WinchControl.Set(0);
		break;
	}
}

Kicker::KickerMotorState Kicker::GetKickerMotorState()
{
	KickerMotorState state = Kicker::Reverse;
	double motorSpeed = m_WinchControl.Get();
	if ( motorSpeed == 0)
	{
		state = Off;
	}
	else if ( motorSpeed > 0)
	{
		state = Forward;
	}
	return state;
}

void Kicker::SetAutoLoad(bool bSet)
{
	Synchronized sync(cKickerStateSemaphore);
	m_bAutoLoad = bSet;
}
bool Kicker::GetAutoLoad()
{
	Synchronized sync(cKickerStateSemaphore);
	return m_bAutoLoad;
}

void Kicker::SetAutoFire(bool bSet)
{
	Synchronized sync(cKickerStateSemaphore);
	m_bAutoFire = bSet;
}

bool Kicker::GetAutoFire()
{
	Synchronized sync(cKickerStateSemaphore);
	return m_bAutoFire;
}

void Kicker::SetKickerState(KickerState state)
{
	Synchronized sync(cKickerStateSemaphore);
	m_KickerState = state;
}

Kicker::KickerState Kicker::GetKickerState()
{
	Synchronized sync(cKickerStateSemaphore);
	return m_KickerState;
}

bool Kicker::IsWinchLoaded()
{
	Synchronized sync(cKickerStateSemaphore);
	return (m_WinchLoaded.Get() == 0);
}

void Kicker::RequestFire()
{
	Synchronized sync(cKickerStateSemaphore);
	m_bFireRequested = true;
}

void Kicker::ClearFireRequest()
{
	Synchronized sync(cKickerStateSemaphore);
	m_bFireRequested = false;	
}

bool Kicker::IsFireRequested()
{
	Synchronized sync(cKickerStateSemaphore);
	return m_bFireRequested;	
}

void Kicker::RequestQuit()
{
	Synchronized sync(cKickerStateSemaphore);
	m_bQuitRequested = true;
}

bool Kicker::IsQuitRequested()
{
	Synchronized sync(cKickerStateSemaphore);
	return m_bQuitRequested;	
}

bool Kicker::IsFireComplete()
{
	Synchronized sync(cKickerStateSemaphore);
	return m_FireCompleteDetector.GetTriggerState();
}

bool Kicker::IsBallDetected()
{
	Synchronized sync(cKickerStateSemaphore);
	return !m_BallDetector.GetTriggerState();
}
