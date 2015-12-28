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
public:
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
		None // Some asteroids are boring. (In reality, most are, but this can't be too realistic a game to be fun).
	};

private:
	FontManager *fontManager;
	
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
	void UpdateTextString(int sentenceId, const char* prefix, float amount, float maxAmount, vmath::vec3& color);
	void UpdateTextStrings();

	float waterRandomLimit;
	float feRandomLimit;
	float siRandomLimit;
	float cuRandomLimit;
	float uRandomLimit;
	float auRandomLimit;
	float ptRandomLimit;
	float impRandomLimit;

public:
	Elementa();
	void Initialize(FontManager *fontManager);
	
	Elements GetRandomElement() const;
	float GetRandomOreAmount(Elements element) const;
	float GetMaxRandomOreAmount(Elements element) const;
	vmath::vec3 GetOreColor(Elements element) const;

	float GetCurrentMass() const;
	void RenderHud(vmath::mat4& perspectiveMatrix);
};

