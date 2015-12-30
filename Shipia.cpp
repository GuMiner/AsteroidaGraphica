#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include "KeyBindingConfig.h"
#include "Shipia.h"
#include "PhysicsConfig.h"

Shipia::Shipia()
{
	shipPosition = vmath::vec3(65, 0, 0);
	shipOrientation = vmath::quaternion(0, 0, 0, 1);

	shipVelocity = vmath::vec3(0, 0, 0);
	shipForce = vmath::vec3(0, 0, 0);
	shipRotation = vmath::vec3(0, 0, 0); // Radians / tick (@ 30 ticks/sec)

	rotationalDampener = false;
	translationalDampener = false;

	rotDampToggled = false;
	tranDampToggled = false;
}

void Shipia::Thrust(bool forwards)
{
	shipForce += (forwards ? PhysicsConfig::ShipThrustSpeed : -PhysicsConfig::ShipThrustSpeed) * shipOrientation.forwardVector();
}

void Shipia::SideThrust(bool left)
{
	vmath::vec3 sidewaysVector = vmath::cross(shipOrientation.upVector(), shipOrientation.forwardVector());
	shipForce += (left ? -PhysicsConfig::ShipSideThrustSpeed : PhysicsConfig::ShipSideThrustSpeed) * sidewaysVector;
}

void Shipia::VerticalThrust(bool up)
{
	shipForce += (up ? -PhysicsConfig::ShipVerticalThrustSpeed : PhysicsConfig::ShipVerticalThrustSpeed) * shipOrientation.upVector();
}

void Shipia::RotateHorizontal(bool left)
{
	shipRotation[0] += left ? -PhysicsConfig::ShipHorizRotSpeed : PhysicsConfig::ShipHorizRotSpeed;

}

void Shipia::RotateVertical(bool up)
{
	shipRotation[1] += up ? -PhysicsConfig::ShipVertRotSpeed : PhysicsConfig::ShipVertRotSpeed;
}

void Shipia::BarrelRoll(bool clockwise)
{
	shipRotation[2] += clockwise ? PhysicsConfig::ShipBarrelRollSpeed : -PhysicsConfig::ShipBarrelRollSpeed;
}

void Shipia::Initialize(SoundManager* soundManager, Elementa* elementa)
{
	this->soundManager = soundManager;
	this->elementa = elementa;
}

// Gets the mass of the ship, used in physics calculations.
float Shipia::GetShipMass() const
{
	return elementa->GetCurrentMass();
}

void Shipia::Update(bool& didRotate, bool& didTranslate)
{
	shipForce = vmath::vec3(0.0f, 0.0f, 0.0f);
	
	didRotate = false;
	didTranslate = false;

	float thrustVolume = 40;
	if (sf::Keyboard::isKeyPressed(KeyBindingConfig::ThrustForwardsKey))
	{
		soundManager->PlaySound(SoundManager::THRUST, sf::Vector3f(0, -3, -5), sf::Vector3f(8, 0, thrustVolume));
		Thrust(true);
		didTranslate = true;
	}

	if (sf::Keyboard::isKeyPressed(KeyBindingConfig::ThrustReverseKey))
	{
		soundManager->PlaySound(SoundManager::THRUST, sf::Vector3f(0, 3, 5), sf::Vector3f(8, 0, thrustVolume));
		Thrust(false);
		didTranslate = true;
	}

	if (sf::Keyboard::isKeyPressed(KeyBindingConfig::ThrustLeftKey))
	{
		// Thrust left and play a right thrust sound.
		soundManager->PlaySound(SoundManager::THRUST, sf::Vector3f(-5, 0, 0), sf::Vector3f(8, 0, thrustVolume));
		SideThrust(true);
		didTranslate = true;
	}

	if (sf::Keyboard::isKeyPressed(KeyBindingConfig::ThrustRightKey))
	{
		// Thrust right, sound on left
		soundManager->PlaySound(SoundManager::THRUST, sf::Vector3f(5, 0, 0), sf::Vector3f(8, 0, thrustVolume));
		SideThrust(false);
		didTranslate = true;
	}

	if (sf::Keyboard::isKeyPressed(KeyBindingConfig::ThrustUpKey))
	{
		// Thrust up, sound below.
		soundManager->PlaySound(SoundManager::THRUST, sf::Vector3f(0, -5, 0), sf::Vector3f(8, 0, thrustVolume));
		VerticalThrust(true);
		didTranslate = true;
	}

	if (sf::Keyboard::isKeyPressed(KeyBindingConfig::ThrustDownKey))
	{
		// Thrust down, sound above.
		soundManager->PlaySound(SoundManager::THRUST, sf::Vector3f(0, 5, 0), sf::Vector3f(8, 0, thrustVolume));
		VerticalThrust(false);
		didTranslate = true;
	}

	if (sf::Keyboard::isKeyPressed(KeyBindingConfig::RotateLeftKey))
	{
		RotateHorizontal(true);
		didRotate = true;
	}

	if (sf::Keyboard::isKeyPressed(KeyBindingConfig::RotateRightKey))
	{
		RotateHorizontal(false);
		didRotate = true;
	}

	if (sf::Keyboard::isKeyPressed(KeyBindingConfig::RotateUpKey))
	{
		RotateVertical(true);
		didRotate = true;
	}

	if (sf::Keyboard::isKeyPressed(KeyBindingConfig::RotateDownKey))
	{
		RotateVertical(false);
		didRotate = true;
	}

	if (sf::Keyboard::isKeyPressed(KeyBindingConfig::RotateCWKey))
	{
		BarrelRoll(true);
		didRotate = true;
	}

	if (sf::Keyboard::isKeyPressed(KeyBindingConfig::RotateCCWKey))
	{
		BarrelRoll(false);
		didRotate = true;
	}

	// If we manually rotated, play the rotation sound
	if (didRotate)
	{
		soundManager->PlaySound(SoundManager::ROTATE, sf::Vector3f(0, 0, 0), sf::Vector3f(8, 0, 20));
	}

	// Toggles on/off the rotational dampener
	if (sf::Keyboard::isKeyPressed(KeyBindingConfig::ToggleRotationDampeningKey) && !rotDampToggled)
	{
		soundManager->PlaySound(SoundManager::BUTTON_TOGGLE, sf::Vector3f(0, 0, 0), sf::Vector3f(5, 0, 100));

		rotationalDampener = !rotationalDampener;
		rotDampToggled = true;
	}
	else if (!sf::Keyboard::isKeyPressed(KeyBindingConfig::ToggleRotationDampeningKey))
	{
		rotDampToggled = false;
	}

	// Toggles on/off the translational dampener
	if (sf::Keyboard::isKeyPressed(KeyBindingConfig::ToggleTranslationDampeningKey) && !tranDampToggled)
	{
		soundManager->PlaySound(SoundManager::BUTTON_TOGGLE, sf::Vector3f(0, 0, 0), sf::Vector3f(5, 0, 100));

		translationalDampener = !translationalDampener;
		tranDampToggled = true;
	}
	else if (!sf::Keyboard::isKeyPressed(KeyBindingConfig::ToggleTranslationDampeningKey))
	{
		tranDampToggled = false;
	}
}

Shipia::~Shipia()
{
}
