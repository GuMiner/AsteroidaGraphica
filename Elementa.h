#pragma once
#include "FontManager.h"
#include "vmath.hpp"

// Holds individual element data
struct Element
{
	// The current amount of each element remaining.
	float amount;

	// Element information for rendering.
	int textId;
	vmath::mat4 textMatrix;
};

class Elementa
{
	FontManager *fontManager;

	enum Elements
	{
		Water, // I know it isn't an element, but it's an essential part of space-life it deserves to be one.
		Fe, // Iron. Used for general ship components.
		Si, // Silicon. Used for electronics.
		Cu, // Copper. Used for electronics. 
		U, // Uranium. Necessary to power the ship; worth $.
		Au, // Gold. Worth $$.
		Pt, // Platinum. Work $$$$.
		Imp, // Impurities. Also not an element, but also essential for the nanites.
	};

	const int textHeight = 20;
	Element water;
	Element fe;
	Element si;
	Element cu;
	Element u;
	Element au;
	Element pt;
	Element imp;

	int totalMassTextId;
	vmath::mat4 totalMassTextMatrix;

	float GetMaximumTotalMass() const;
	float GetCurrentMass() const;
	void UpdateTextString(int sentenceId, const char* prefix, float amount, float maxAmount, vmath::vec3& color);
	void UpdateTextStrings();

public:
	Elementa();
	void Initialize(FontManager *fontManager);
	void RenderHud(vmath::mat4& perspectiveMatrix);
};

