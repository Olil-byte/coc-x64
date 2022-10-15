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
	Fmatrix* rotation = static_cast<Fmatrix*>(P->callback_param());
	P->mTransform.mulB_43(*rotation);
}

void CWeaponDP28::SetMagazineBoneCallback()
{
	attachable_hud_item* hi = HudItemData();

	u16 boneId = hi->m_model->LL_BoneID(m_sMagazineBone);
	CBoneInstance& magazineBone = hi->m_model->LL_GetBoneInstance(boneId);

	magazineBone.set_callback(bctCustom, &MagazineBoneCallback, &m_mMagazineRotation);
}

void CWeaponDP28::OnShot()
{
	inherited::OnShot();

	m_fMagazineRotationTime += m_fMagazineRotationStep / m_fMagazineRotationSpeed;
}

void CWeaponDP28::UnloadMagazine(bool spawn_ammo)
{
	inherited::UnloadMagazine(spawn_ammo);
	RecalculateHudMagazineRotation();
}

void CWeaponDP28::OnMotionMark(u32 state, const motion_marks& M)
{
	inherited::OnMotionMark(state, M);
	if (state == eReload)
	{
		ReloadMagazine();
		RecalculateHudMagazineRotation();
	}
}

void CWeaponDP28::UpdateHudAdditonal(Fmatrix& trans)
{
	inherited::UpdateHudAdditonal(trans);

	UpdateHudMagazineRotation();
}

void CWeaponDP28::UpdateHudMagazineRotation()
{
	SetMagazineBoneCallback();

	if (m_fMagazineRotationTime > 0.0f)
	{
		CalculateHudMagazineRotation(m_fMagazineRotationSpeed * Device.fTimeDelta);

		m_fMagazineRotationTime -= Device.fTimeDelta;
	}
}

void CWeaponDP28::CalculateHudMagazineRotation(float value)
{
	Fmatrix rotation;
	rotation.identity();
	rotation.rotation(m_vMagazineRotationAxis, value);

	m_mMagazineRotation.mulB_43(rotation);
}

void CWeaponDP28::RecalculateHudMagazineRotation()
{
	m_fMagazineRotationTime = 0.0f;
	m_mMagazineRotation.identity();

	CalculateHudMagazineRotation(PI_MUL_2 - m_fMagazineRotationStep * iAmmoElapsed);
}

void CWeaponDP28::on_a_hud_attach()
{
	inherited::on_a_hud_attach();
}

void CWeaponDP28::on_b_hud_detach()
{
	inherited::on_b_hud_detach();

	attachable_hud_item* hi = HudItemData();

	u16 boneId = hi->m_model->LL_BoneID(m_sMagazineBone);
	CBoneInstance& magazineBone = hi->m_model->LL_GetBoneInstance(boneId);

	magazineBone.reset_callback();
}
