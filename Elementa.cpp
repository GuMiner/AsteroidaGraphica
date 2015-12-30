#include <string>
#include <sstream>
#include "Constants.h"
#include "Elementa.h"
#include "Logger.h"
#include "OreConfig.h"
#include "PhysicsConfig.h"

Elementa::Elementa()
{ }

// Loads in the base ore amounts and initializes the inventory text strings.
void Elementa::Initialize(FontManager *fontManager)
{
	this->fontManager = fontManager;

	Logger::Log("Loading in the base ore amounts...");
	water.amount = OreConfig::BaseWaterOre;
	fe.amount = OreConfig::BaseFeOre;
	si.amount = OreConfig::BaseSiOre;
	cu.amount = OreConfig::BaseCuOre;
	u.amount = OreConfig::BaseUOre;
	au.amount = OreConfig::BaseAuOre;
	pt.amount = OreConfig::BasePtOre;
	imp.amount = OreConfig::BaseImpOre;

	water.textId = fontManager->CreateNewSentence();
	fe.textId = fontManager->CreateNewSentence();
	si.textId = fontManager->CreateNewSentence();
	cu.textId = fontManager->CreateNewSentence();
	u.textId = fontManager->CreateNewSentence();
	au.textId = fontManager->CreateNewSentence();
	pt.textId = fontManager->CreateNewSentence();
	imp.textId = fontManager->CreateNewSentence();
	totalMassTextId = fontManager->CreateNewSentence();

	vmath::mat4 textScale = vmath::scale(0.015f, 0.015f, 0.015f);
	water.textMatrix = vmath::translate(0.611f, 0.420f, -1.0f) * textScale;
	fe.textMatrix = vmath::translate(0.611f, 0.391f, -1.0f) * textScale;
	si.textMatrix = vmath::translate(0.611f, 0.362f, -1.0f) * textScale;
	cu.textMatrix = vmath::translate(0.611f, 0.333f, -1.0f) * textScale;
	u.textMatrix = vmath::translate(0.611f, 0.304f, -1.0f) * textScale;
	au.textMatrix = vmath::translate(0.611f, 0.275f, -1.0f) * textScale;
	pt.textMatrix = vmath::translate(0.611f, 0.246f, -1.0f) * textScale;
	imp.textMatrix = vmath::translate(0.611f, 0.217f, -1.0f) * textScale;
	totalMassTextMatrix = vmath::translate(0.611f, 0.188f, -1.0f) * textScale;

	UpdateTextStrings();

	// Setup the random limits for weighted random element generation.
	float sumElementWeights = OreConfig::WaterRatio + OreConfig::FeRatio + OreConfig::SiRatio +
		OreConfig::CuRatio + OreConfig::URatio + OreConfig::AuRatio + OreConfig::PtRatio + OreConfig::ImpRatio;

	waterRandomLimit = OreConfig::WaterRatio / sumElementWeights;
	feRandomLimit = waterRandomLimit + OreConfig::FeRatio / sumElementWeights;
	siRandomLimit = feRandomLimit + OreConfig::SiRatio / sumElementWeights;
	cuRandomLimit = siRandomLimit + OreConfig::CuRatio / sumElementWeights;
	uRandomLimit = cuRandomLimit + OreConfig::URatio / sumElementWeights;
	auRandomLimit = uRandomLimit + OreConfig::AuRatio / sumElementWeights;
	ptRandomLimit = auRandomLimit + OreConfig::PtRatio / sumElementWeights;
	impRandomLimit = ptRandomLimit + OreConfig::ImpRatio / sumElementWeights; // 1.00f.
}

// Gets the maximum total mass the ship can carry, + the ship mass.
float Elementa::GetMaximumTotalMass() const
{
	return PhysicsConfig::BaseShipMass +
		OreConfig::MaxWaterOre + OreConfig::MaxFeOre + OreConfig::MaxSiOre + OreConfig::MaxCuOre +
		OreConfig::MaxUOre + OreConfig::MaxAuOre + OreConfig::MaxPtOre + OreConfig::MaxImpOre;
}

#define RandomSelectorElseIf(element, Element)		  \
	else if (randomSelector < ##element##RandomLimit) \
	{												  \
		return Elements::##Element;					  \
	}												  \

// Returns a random element, properly accounting for weighting.
Elementa::Elements Elementa::GetRandomElement() const
{
	float randomSelector = Constants::Rand();
	if (randomSelector < waterRandomLimit)
	{
		return Elements::Water;
	}
	RandomSelectorElseIf(fe, Fe)
	RandomSelectorElseIf(si, Si)
	RandomSelectorElseIf(cu, Cu)
	RandomSelectorElseIf(u, U)
	RandomSelectorElseIf(au, Au)
	RandomSelectorElseIf(pt, Pt)
	else
	{
		return Elements::Imp;
	}
}

// Simplifies repetative cases for random ore amounts.
#define RandomOreAmount(Element)								 \
	case Elements::##Element:									 \
		return Constants::Rand()*OreConfig::##Element##Ratio \

// Gets a random ore-specific ore amount.
float Elementa::GetRandomOreAmount(Elementa::Elements element) const
{
	switch (element)
	{
		RandomOreAmount(Water);
		RandomOreAmount(Fe);
		RandomOreAmount(Si);
		RandomOreAmount(Cu);
		RandomOreAmount(U);
		RandomOreAmount(Au);
		RandomOreAmount(Pt);
		RandomOreAmount(Imp);
	}

	return 0.0f;
}

// Simplifies max ore amount lookups.
#define MaxOreAmount(Element)					\
	case Elements::##Element:					\
		return OreConfig::##Element##Ratio  \


// Gets the maximum random ore-specific ore amount.
float Elementa::GetMaxRandomOreAmount(Elementa::Elements element) const
{
	switch(element)
	{
		MaxOreAmount(Water);
		MaxOreAmount(Fe);
		MaxOreAmount(Si);
		MaxOreAmount(Cu);
		MaxOreAmount(U);
		MaxOreAmount(Au);
		MaxOreAmount(Pt);
		MaxOreAmount(Imp);
	}

	return 1.0f;
}

// Simplifies repetative cases for colors.
#define ElementColorCase(Element)			     \
	case Elements::##Element:				     \
	   return OreConfig::##Element##OreColor \

//Gets the ore color for the respective element.
vmath::vec3 Elementa::GetOreColor(Elementa::Elements element) const
{
	switch (element)
	{
		ElementColorCase(Water);
		ElementColorCase(Fe);
		ElementColorCase(Si);
		ElementColorCase(Cu);
		ElementColorCase(U);
		ElementColorCase(Au);
		ElementColorCase(Pt);
		ElementColorCase(Imp);
	}

	return vmath::vec3(1.0f, 1.0f, 1.0f);
}

// Gets the current mass of the ship + elements.
float Elementa::GetCurrentMass() const
{
	return PhysicsConfig::BaseShipMass +
		water.amount + fe.amount + si.amount + cu.amount + u.amount + au.amount + pt.amount + imp.amount;
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
	UpdateTextString(water.textId, "Water: ", water.amount, OreConfig::MaxWaterOre, OreConfig::WaterOreColor);
	UpdateTextString(fe.textId, "Iron: ", fe.amount, OreConfig::MaxFeOre, OreConfig::FeOreColor);
	UpdateTextString(si.textId, "Silicon: ", si.amount, OreConfig::MaxSiOre, OreConfig::SiOreColor);
	UpdateTextString(cu.textId, "Copper: ", cu.amount, OreConfig::MaxCuOre, OreConfig::CuOreColor);
	UpdateTextString(u.textId, "Uranium: ", u.amount, OreConfig::MaxUOre, OreConfig::UOreColor);
	UpdateTextString(au.textId, "Gold: ", au.amount, OreConfig::MaxAuOre, OreConfig::AuOreColor);
	UpdateTextString(pt.textId, "Platinum: ", pt.amount, OreConfig::MaxPtOre, OreConfig::PtOreColor);
	UpdateTextString(imp.textId, "Impurities: ", imp.amount, OreConfig::MaxImpOre, OreConfig::ImpOreColor);

	UpdateTextString(totalMassTextId, "Total: ", GetCurrentMass(), GetMaximumTotalMass(), vmath::vec3(1.0f, 1.0f, 1.0f));
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

	fontManager->RenderSentence(totalMassTextId, perspectiveMatrix, totalMassTextMatrix);
}
