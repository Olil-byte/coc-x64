#include "stdafx.h"
#include "BoxMagazine.h"

BoxMagazine::BoxMagazine()
{

}

BoxMagazine::~BoxMagazine()
{

}

void BoxMagazine::Load(LPCSTR section)
{
	MagazineBase::Load(section);

	size_t capacity = pSettings->r_u64(section, "capacity");
	if (capacity < 0) 
	{
		throw "Magazine capacity must be greater than zero";
	}
	m_rounds.reserve(capacity);
}

void BoxMagazine::Push(CCartridge&& cartridge)
{
	m_rounds.emplace_back(cartridge);
}

const CCartridge& BoxMagazine::GetTop()
{
	return m_rounds.back();
}

CCartridge&& BoxMagazine::Pop()
{
	CCartridge cartridge = std::move(m_rounds.back());
	m_rounds.pop_back();
	return std::move(cartridge);
}

size_t BoxMagazine::GetCapacity() const
{
	return m_rounds.capacity();
}

bool BoxMagazine::IsEmpty() const
{
	return m_rounds.empty();
}
