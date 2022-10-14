#include "stdafx.h"
#include "WeaponDP28.h"
#include "player_hud.h"

CWeaponDP28::CWeaponDP28(ESoundTypes eSoundType) : inherited(eSoundType)
{
	m_sMagazineBone = nullptr;
	m_fMagazineRotationFactor = 0.0f;
	m_fMagazineRotationSpeedFactor = 0.0f;
	m_mMagazineRotation.identity();
	m_fMagazineRotationTime = 0.0f;
}

CWeaponDP28::~CWeaponDP28()
{
}

void CWeaponDP28::Load(LPCSTR section) 
{
	inherited::Load(section);

	m_sMagazineBone = pSettings->r_string(section, "magazine_bone");
	m_vMagazineRotationAxis = pSettings->r_fvector3(section, "magazine_rotate_axis");

	if(pSettings->line_exist(section, "magazine_rotation_factor"))
	{
		m_fMagazineRotationFactor = pSettings->r_float(section, "magazine_rotation_factor");
	}
	else 
	{
		m_fMagazineRotationFactor = PI_MUL_2 / iMagazineSize;
	}

	m_fMagazineRotationSpeedFactor = pSettings->r_float(section, "magazine_rotation_speed_factor");
}

void CWeaponDP28::OnShot()
{
	inherited::OnShot();

	m_fMagazineRotationTime += 1.0f / m_fMagazineRotationSpeedFactor;
}

void CWeaponDP28::UpdateHudAdditonal(Fmatrix& trans)
{
	inherited::UpdateHudAdditonal(trans);

	if (m_fMagazineRotationTime > 0.0f) 
	{
		Fmatrix rotation;
		rotation.identity();
		rotation.rotation(m_vMagazineRotationAxis, m_fMagazineRotationFactor * m_fMagazineRotationSpeedFactor * Device.fTimeDelta);

		m_mMagazineRotation.mulB_43(rotation);

		m_fMagazineRotationTime -= Device.fTimeDelta;
		clamp(m_fMagazineRotationTime, 0.0f, FLT_MAX);
	}
}

void _BCL MagazineBoneCallback(CBoneInstance* P)
{
	Fmatrix* rotation = static_cast<Fmatrix*>(P->callback_param());
	P->mTransform.mulB_43(*rotation);
}

void CWeaponDP28::on_a_hud_attach()
{
	inherited::on_a_hud_attach();

	attachable_hud_item* hi = HudItemData();

	u16 boneId = hi->m_model->LL_BoneID(m_sMagazineBone);
	CBoneInstance& magazineBone = hi->m_model->LL_GetBoneInstance(boneId);

	magazineBone.set_callback(bctCustom, &MagazineBoneCallback, &m_mMagazineRotation);
}

void CWeaponDP28::on_b_hud_detach()
{
	inherited::on_b_hud_detach();

	attachable_hud_item* hi = HudItemData();

	u16 boneId = hi->m_model->LL_BoneID(m_sMagazineBone);
	CBoneInstance& magazineBone = hi->m_model->LL_GetBoneInstance(boneId);

	magazineBone.reset_callback();
}
