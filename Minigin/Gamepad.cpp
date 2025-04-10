#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Gamepad.h"
#include "XInput.h"
class dae::GamePadImpl {
public:
	GamePadImpl() {

	}
	GamePadImpl(int a_iIndex): m_ControllerIndex{ static_cast<DWORD>(a_iIndex) }
	{

	}
	int GetIndex()
	{
		return m_ControllerIndex;
	}

	bool Connected()
	{
		XINPUT_STATE checkState;
		ZeroMemory(&checkState, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		auto dwResult = XInputGetState(m_ControllerIndex, &checkState);
		if (dwResult == ERROR_SUCCESS)
		{
			return true;
		}
		else {
			return false;
		}
	}

	void Update()
	{
		if (Connected())
		{
			XINPUT_STATE previousState;
			CopyMemory(&previousState, &m_CurrentState, sizeof(XINPUT_STATE));
			ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
			XInputGetState(m_ControllerIndex, &m_CurrentState);
			// Controller is connected
			auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
			m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
			m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
		}
		else
		{
			// Controller is not connected
		}
	}

	void SetUsed(bool used)
	{
		m_Used = used;
	}

	bool IsUsed()
	{
		return m_Used;
	}

	bool IsDownThisFrame(unsigned int button) const
	{
		return m_ButtonsPressedThisFrame & button;
	}

	bool IsUpThisFrame(unsigned int button) const
	{
		return m_ButtonsReleasedThisFrame & button;
	}

	bool IsPressed(unsigned int button) const
	{
		return m_CurrentState.Gamepad.wButtons & button;
	}
private:
	XINPUT_STATE m_CurrentState;
	bool m_Used;
	int m_ButtonsPressedThisFrame;
	int m_ButtonsReleasedThisFrame;
	DWORD m_ControllerIndex;
};


dae::Gamepad::Gamepad(): pimpl{std::make_unique<GamePadImpl>()}
{
}

dae::Gamepad::Gamepad(int a_iIndex): pimpl(std::make_unique<GamePadImpl>(a_iIndex))
{
}

int dae::Gamepad::GetIndex()
{
	return pimpl->GetIndex();
}

bool dae::Gamepad::Connected()
{
	return pimpl->Connected();
}

void dae::Gamepad::Update()
{
	pimpl->Update();
}

void dae::Gamepad::SetUsed(bool used)
{
	pimpl->SetUsed(used);
}

bool dae::Gamepad::IsUsed()
{
	return pimpl->IsUsed();
}

bool dae::Gamepad::IsDownThisFrame(unsigned int button) const
{
	return pimpl->IsDownThisFrame(button);
}

bool dae::Gamepad::IsUpThisFrame(unsigned int button) const
{
	return pimpl->IsUpThisFrame(button);
}

bool dae::Gamepad::IsPressed(unsigned int button) const
{
	return pimpl->IsPressed(button);
}
