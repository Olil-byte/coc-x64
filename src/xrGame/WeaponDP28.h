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

	virtual void UnloadMagazine(bool spawn_ammo = true);

	virtual void OnMotionMark(u32 state, const motion_marks& M);

protected:
	static void _BCL MagazineBoneCallback(CBoneInstance* P);
	virtual void SetMagazineBoneCallback();

	virtual void UpdateHudAdditonal(Fmatrix& trans);
	virtual void UpdateHudMagazineRotation();

	virtual void CalculateHudMagazineRotation(float value);
	virtual void RecalculateHudMagazineRotation();

	virtual void on_a_hud_attach();
	virtual void on_b_hud_detach();

private:
	shared_str m_sMagazineBone;

	float m_fMagazineRotationStep;
	float m_fMagazineRotationSpeed;
	float m_fMagazineRotationTime;

	Fvector m_vMagazineRotationAxis;
	Fmatrix m_mMagazineRotation;

protected:
	DECLARE_SCRIPT_REGISTER_FUNCTION
};

add_to_type_list(CWeaponDP28)
#undef script_type_list
#define script_type_list save_type_list(CWeaponDP28)
