#pragma once

#include "WeaponMagazined.h"
#include "../Include/xrRender/Kinematics.h"
#include "script_export_space.h"

class CWeaponDP28: public CWeaponMagazined
{
private:
	using inherited = CWeaponMagazined;
public:
	CWeaponDP28(ESoundTypes eSoundType = SOUND_TYPE_WEAPON_SUBMACHINEGUN);
	virtual ~CWeaponDP28();

	virtual void Load(LPCSTR section);

	virtual void OnShot();

	virtual void UpdateHudAdditonal(Fmatrix& trans);

	virtual void on_a_hud_attach();
	virtual void on_b_hud_detach();

private:
	shared_str m_sMagazineBone;

	float m_fMagazineRotationFactor;
	float m_fMagazineRotationSpeedFactor;
	float m_fMagazineRotationTime;

	Fvector m_vMagazineRotationAxis;
	Fmatrix m_mMagazineRotation;

protected:
	DECLARE_SCRIPT_REGISTER_FUNCTION
};

add_to_type_list(CWeaponDP28)
#undef script_type_list
#define script_type_list save_type_list(CWeaponDP28)
