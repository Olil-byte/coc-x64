#include "stdafx.h"
#include "MagazineBase.h"

MagazineBase::~MagazineBase()
{
}

void MagazineBase::Load(LPCSTR section)
{
	CInventoryItemObject::Load(section);
}
