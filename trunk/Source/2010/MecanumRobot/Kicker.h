#ifndef _KICKER_H
#define _KICKER_H

#include "WPILib.h"

class Kicker
{
public:
	
	enum KickerMotorState { Forward, Reverse, Off };
	
	enum KickerState { Loading, Loaded, Unloaded }; 
	
	Kicker( UINT32 iJaguarPort, 
			UINT32 iDigitalWinchLoadedDetectorPort,
			UINT32 iDigitalBallDetectorPort
			);
	~Kicker();
	
	void GetStatus( float *fireCompleteDetectorValue, 
					bool *fireCompleteDetected,
					float *ballDetectorValue,
					bool *ballDetected,
					bool *winchLoaded,
					KickerMotorState* state,
					bool *autoLoad,
					bool *autoFire);
	
	void SetKickerMotorState( KickerMotorState state );
	KickerMotorState GetKickerMotorState();
	void SetAutoLoad(bool bSet);
	bool GetAutoLoad();
	void SetAutoFire(bool bSet);
	bool GetAutoFire();
	void SetKickerState(KickerState state);
	KickerState GetKickerState();
	bool IsWinchLoaded();
	void RequestQuit();
	bool IsQuitRequested();
	void RequestFire();
	bool IsFireRequested();
	bool IsBallDetected();
	void ClearFireRequest();
	
protected:
	Jaguar m_WinchControl;
	DigitalInput m_WinchLoaded;
	DigitalInput m_BallDetected;
	bool		  m_bAutoLoad;
	bool		  m_bAutoFire;
	KickerState	  m_KickerState;

	bool		  m_bFireRequested;
	bool          m_bQuitRequested;
	Task 		  m_task;

};

#endif // _KICKER_H

	
	
