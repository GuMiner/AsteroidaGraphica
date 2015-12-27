#include <string>
#include <sstream>
#include "ConfigManager.h"
#include "Elementa.h"
#include "Logger.h"

Elementa::Elementa()
{ }

// Loads in the base ore amounts and initializes the inventory text strings.
void Elementa::Initialize(FontManager *fontManager)
{
	this->fontManager = fontManager;

	Logger::Log("Loading in the base ore amounts...");
	water.amount = ConfigManager::BaseWaterOre;
	fe.amount = ConfigManager::BaseFeOre;
	si.amount = ConfigManager::BaseSiOre;
	cu.amount = ConfigManager::BaseCuOre;
	u.amount = ConfigManager::BaseUOre;
	au.amount = ConfigManager::BaseAuOre;
	pt.amount = ConfigManager::BasePtOre;
	imp.amount = ConfigManager::BaseImpOre;

	water.textId = fontManager->CreateNewSentence();
	fe.textId = fontManager->CreateNewSentence();
	si.textId = fontManager->CreateNewSentence();
	cu.textId = fontManager->CreateNewSentence();
	u.textId = fontManager->CreateNewSentence();
	au.textId = fontManager->CreateNewSentence();
	pt.textId = fontManager->CreateNewSentence();
	imp.textId = fontManager->CreateNewSentence();

	vmath::mat4 textScale = vmath::scale(0.015f, 0.015f, 0.015f);
	water.textMatrix = vmath::translate(0.611f, 0.420f, -1.0f) * textScale;
	fe.textMatrix = vmath::translate(0.611f, 0.391f, -1.0f) * textScale;
	si.textMatrix = vmath::translate(0.611f, 0.362f, -1.0f) * textScale;
	cu.textMatrix = vmath::translate(0.611f, 0.333f, -1.0f) * textScale;
	u.textMatrix = vmath::translate(0.611f, 0.304f, -1.0f) * textScale;
	au.textMatrix = vmath::translate(0.611f, 0.275f, -1.0f) * textScale;
	pt.textMatrix = vmath::translate(0.611f, 0.246f, -1.0f) * textScale;
	imp.textMatrix = vmath::translate(0.611f, 0.217f, -1.0f) * textScale;

	UpdateTextStrings();
}

// Updates a single inventory text string.
void Elementa::UpdateTextString(int sentenceId, const char* prefix, float amount, float maxAmount, vmath::vec3& color)
{
	std::stringstream sentence;
	sentence.precision(1);
	sentence << std::fixed;

	sentence << prefix << amount << "/" << maxAmount << " kg.";
	fontManager->UpdateSentence(sentenceId, sentence.str(), textHeight, color);
}

// Updates the inventory text strings based on the current ore amounts and maximum ore amounts.
void Elementa::UpdateTextStrings()
{
	UpdateTextString(water.textId, "Water: ", water.amount, ConfigManager::MaxWaterOre, ConfigManager::WaterOreColor);
	UpdateTextString(fe.textId, "Iron: ", fe.amount, ConfigManager::MaxFeOre, ConfigManager::FeOreColor);
	UpdateTextString(si.textId, "Silicon: ", si.amount, ConfigManager::MaxSiOre, ConfigManager::SiOreColor);
	UpdateTextString(cu.textId, "Copper: ", cu.amount, ConfigManager::MaxCuOre, ConfigManager::CuOreColor);
	UpdateTextString(u.textId, "Uranium: ", u.amount, ConfigManager::MaxUOre, ConfigManager::UOreColor);
	UpdateTextString(au.textId, "Gold: ", au.amount, ConfigManager::MaxAuOre, ConfigManager::AuOreColor);
	UpdateTextString(pt.textId, "Platinum: ", pt.amount, ConfigManager::MaxPtOre, ConfigManager::PtOreColor);
	UpdateTextString(imp.textId, "Impurities: ", imp.amount, ConfigManager::MaxImpOre, ConfigManager::ImpOreColor);
}

// Renders all of the inventory amounts.
void Elementa::RenderHud(vmath::mat4 & perspectiveMatrix)
{
	fontManager->RenderSentence(water.textId, perspectiveMatrix, water.textMatrix);
	fontManager->RenderSentence(fe.textId, perspectiveMatrix, fe.textMatrix);
	fontManager->RenderSentence(si.textId, perspectiveMatrix, si.textMatrix);
	fontManager->RenderSentence(cu.textId, perspectiveMatrix, cu.textMatrix);
	fontManager->RenderSentence(u.textId, perspectiveMatrix, u.textMatrix);
	fontManager->RenderSentence(au.textId, perspectiveMatrix, au.textMatrix);
	fontManager->RenderSentence(pt.textId, perspectiveMatrix, pt.textMatrix);
	fontManager->RenderSentence(imp.textId, perspectiveMatrix, imp.textMatrix);
}

Elementa::~Elementa()
{
}
