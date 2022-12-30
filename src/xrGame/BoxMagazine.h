#pragma once

#include "MagazineBase.h"

class BoxMagazine: public MagazineBase
{
public:
	BoxMagazine();
	virtual ~BoxMagazine();

	virtual void Load(LPCSTR section);

	virtual void Push(CCartridge&& cartridge);
	virtual const CCartridge& GetTop();
	virtual CCartridge&& Pop();

	virtual size_t GetCapacity() const;
	virtual bool IsEmpty() const;

private:
	xr_vector<CCartridge> m_rounds;
};

