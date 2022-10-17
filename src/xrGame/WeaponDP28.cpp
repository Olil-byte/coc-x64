#include "stdafx.h"
#include "WeaponDP28.h"
#include "player_hud.h"

CWeaponDP28::CWeaponDP28(ESoundTypes eSoundType) : inherited(eSoundType)
{
	m_sMagazineBone = nullptr;

	m_fMagazineRotationStep = 0.0f;
	m_fMagazineRotationSpeed = 0.0f;
	m_fMagazineRotationTime = 0.0f;

	m_mMagazineRotation.identity();
}

CWeaponDP28::~CWeaponDP28()
{
}

void CWeaponDP28::Load(LPCSTR section) 
{
	inherited::Load(section);

	m_sMagazineBone = pSettings->r_string(section, "magazine_bone");
	m_vMagazineRotationAxis = pSettings->r_fvector3(section, "magazine_rotate_axis");

	m_fMagazineRotationStep = PI_MUL_2 / iMagazineSize;
	m_fMagazineRotationSpeed = pSettings->r_float(section, "magazine_rotation_speed");
}

void _BCL CWeaponDP28::MagazineBoneCallback(CBoneInstance* P)
{
	CWeaponDP28* weapon = static_cast<CWeaponDP28*>(P->callback_param());
	P->mTransform.mulB_43(weapon->m_mMagazineRotation);
}

void CWeaponDP28::SetMagazineBoneCallback()
{
	IKinematics* worldVisual = smart_cast<IKinematics*>(Visual());

	u16 boneId = worldVisual->LL_BoneID(m_sMagazineBone);
	CBoneInstance& magazineBone = worldVisual->LL_GetBoneInstance(boneId);

	magazineBone.set_callback(bctCustom, &MagazineBoneCallback, this);
}

void CWeaponDP28::ResetMagazineBoneCallback()
{
	IKinematics* worldVisual = smart_cast<IKinematics*>(Visual());

	u16 boneId = worldVisual->LL_BoneID(m_sMagazineBone);
	CBoneInstance& magazineBone = worldVisual->LL_GetBoneInstance(boneId);

	magazineBone.reset_callback();
}

void CWeaponDP28::SetHudMagazineBoneCallback()
{
	attachable_hud_item* hudItem = HudItemData();
	IKinematics* hudVisual = hudItem->m_model;

	u16 boneId = hudVisual->LL_BoneID(m_sMagazineBone);
	CBoneInstance& magazineBone = hudVisual->LL_GetBoneInstance(boneId);

	magazineBone.set_callback(bctCustom, &MagazineBoneCallback, this);
}

void CWeaponDP28::ResetHudMagazineBoneCallback()
{
	attachable_hud_item* hudItem = HudItemData();
	IKinematics* hudVisual = hudItem->m_model;

	u16 boneId = hudVisual->LL_BoneID(m_sMagazineBone);
	CBoneInstance& magazineBone = hudVisual->LL_GetBoneInstance(boneId);

	magazineBone.reset_callback();
}

void CWeaponDP28::OnShot()
{
	inherited::OnShot();

	m_fMagazineRotationTime += m_fMagazineRotationStep / m_fMagazineRotationSpeed;
}

void CWeaponDP28::UnloadMagazine(bool spawn_ammo)
{
	inherited::UnloadMagazine(spawn_ammo);
	RecalculateMagazineRotation();
}

void CWeaponDP28::OnMotionMark(u32 state, const motion_marks& M)
{
	inherited::OnMotionMark(state, M);
	if (state == eReload)
	{
		ReloadMagazine();
		RecalculateMagazineRotation();
	}
}

void CWeaponDP28::UpdateCL()
{
	inherited::UpdateCL();

	UpdateMagazineRotation();

	//IKinematics* worldVisual = smart_cast<IKinematics*>(Visual());

	//u16 boneId = worldVisual->LL_BoneID(m_sMagazineBone);
	//CBoneInstance& magazineBone = worldVisual->LL_GetBoneInstance(boneId);

	//magazineBone.set_callback(bctCustom, &MagazineBoneCallback, this);
}

void CWeaponDP28::UpdateMagazineRotation()
{
	if (m_fMagazineRotationTime > 0.0f)
	{
		CalculateMagazineRotation(m_fMagazineRotationSpeed * Device.fTimeDelta);

		m_fMagazineRotationTime -= Device.fTimeDelta;
	}
}

void CWeaponDP28::CalculateMagazineRotation(float value)
{
	Fmatrix rotation;
	rotation.identity();
	rotation.rotation(m_vMagazineRotationAxis, value);

	m_mMagazineRotation.mulB_43(rotation);
}

void CWeaponDP28::RecalculateMagazineRotation()
{
	m_fMagazineRotationTime = 0.0f;
	m_mMagazineRotation.identity();

	CalculateMagazineRotation(PI_MUL_2 - m_fMagazineRotationStep * iAmmoElapsed);
}

BOOL CWeaponDP28::net_Spawn(CSE_Abstract* DC)
{
	BOOL bResult = inherited::net_Spawn(DC);

	RecalculateMagazineRotation();

	SetMagazineBoneCallback();

	return bResult;
}

void CWeaponDP28::net_Destroy()
{
	ResetMagazineBoneCallback();
	inherited::net_Destroy();
}

void CWeaponDP28::OnH_B_Chield()
{
	inherited::OnH_B_Chield();
	ResetMagazineBoneCallback();
}

void CWeaponDP28::OnH_A_Chield()
{
	inherited::OnH_A_Chield();
	SetMagazineBoneCallback();
}

void CWeaponDP28::OnH_B_Independent(bool just_before_destroy)
{
	inherited::OnH_B_Independent(just_before_destroy);
	ResetMagazineBoneCallback();
}

void CWeaponDP28::OnH_A_Independent()
{
	inherited::OnH_A_Independent();
	SetMagazineBoneCallback();
}

void CWeaponDP28::on_a_hud_attach() 
{
	inherited::on_a_hud_attach();
	SetHudMagazineBoneCallback();
}

void CWeaponDP28::on_b_hud_detach()
{
	inherited::on_b_hud_detach();
	ResetHudMagazineBoneCallback();
}
