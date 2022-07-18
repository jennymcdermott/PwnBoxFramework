#include "Plants.h"
#include <vector>
#include <Windows.h>
#include <PwnBoxFramework.h>
namespace PwnBoxFramework
{

	void plants::init() noexcept
	{
		m_interface = memory->GetPointerAddress(m_gameModule, { 0x868 });
		m_pMemory.plantBaseAddr = memory->GetPointerAddress(m_interface, { 0xC4 });
		m_pMemory.plantHealthAddr = memory->GetPointerAddress(m_pMemory.plantBaseAddr, { 0x40 });
		m_pMemory.plantMaxHPAddr = memory->GetPointerAddress(m_pMemory.plantBaseAddr, { 0x44 });
		m_pMemory.plantRechargeAddr = memory->GetPointerAddress(m_pMemory.plantBaseAddr, { 0x54 });
		m_pMemory.plantCurrentCount = memory->GetPointerAddress(m_interface, { 0xD4 });
	}

	plants::plants(const int& gameModule) noexcept :
		m_gameModule(gameModule) {
		init();
	}

	int plants::getPlantHealth() noexcept { return memory->ReadProcMemory<int>(m_pMemory.plantHealthAddr); }
	int plants::getPlantMaxHP() noexcept { return memory->ReadProcMemory<int>(m_pMemory.plantMaxHPAddr); }
	int plants::getPlantId() noexcept { return 0; }
	int plants::getCurrentPlantCount() noexcept { return memory->ReadProcMemory<int>(m_pMemory.plantCurrentCount); }
	int plants::getPlantRechargeTime() noexcept { return memory->ReadProcMemory<int>(m_pMemory.plantRechargeAddr); }

	float plants::getPlantX(int index) noexcept { return 0.0f; }
	float plants::getPlantY(int index) noexcept { return 0.0f; }

	void plants::OnUpdate() noexcept
	{
		m_interface = memory->GetPointerAddress(m_gameModule, { 0x868 });
		m_pMemory.plantBaseAddr = memory->GetPointerAddress(m_interface, { 0xC4 });
		m_pMemory.plantHealthAddr = memory->GetPointerAddress(m_pMemory.plantBaseAddr, { 0x40 });
		m_pMemory.plantMaxHPAddr = memory->GetPointerAddress(m_pMemory.plantBaseAddr, { 0x44 });
		m_pMemory.plantRechargeAddr = memory->GetPointerAddress(m_pMemory.plantBaseAddr, { 0x54 });
		m_pMemory.plantCurrentCount = memory->GetPointerAddress(m_interface, { 0xD4 });
	}
}