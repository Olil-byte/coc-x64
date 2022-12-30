#pragma once

#include "inventory_item_object.h"
#include "WeaponAmmo.h"

class IMagazine
{
public:
	virtual ~IMagazine() {};

	virtual void Push(CCartridge&& cartridge) = 0;
	virtual const CCartridge& GetTop() = 0;
	virtual CCartridge&& Pop() = 0;

	virtual size_t GetCapacity() const = 0;
	virtual bool IsEmpty() const = 0;
};

class MagazineBase: public CInventoryItemObject, public IMagazine
{
public:
	virtual ~MagazineBase();

	virtual void Load(LPCSTR section);

	virtual void Push(CCartridge&& cartridge) = 0;
	virtual const CCartridge& GetTop() = 0;
	virtual CCartridge&& Pop() = 0;

	virtual size_t GetCapacity() const = 0;
	virtual bool IsEmpty() const = 0;

private:
	xr_vector<CCartridge> m_rounds;
};

