#include "Pch.h"

#include <string>
#include <sstream>
#include <cstdlib>


int randRangeInt(int min, int max)
{
    return min + rand () % (max - min + 1);
}

double randRangeDouble(double min, double max)
{
    return min + static_cast<double>(rand()) / RAND_MAX * (max - min);
}

Color getRandomPotColor()
{
	int choice = std::rand() % 4 + 1;
    
	if (choice == 1) return Cpot1;
	if (choice == 2) return Cpot2;
	if (choice == 3) return Cpot3;
	if (choice == 4) return Cpot4;
    return Color();
}

String getSoundAccordingToColor(Color col)
{
    if (col == Cpot1) return Spot1;
    if (col == Cpot2) return Spot2;
    if (col == Cpot3) return Spot3;
    if (col == Cpot4) return Spot4;
    return "";
}

std::string toStringInt(int value)
{
    std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string toStringDouble(double value)
{
    std::stringstream ss;
	ss << value;
	return ss.str();
}

String getTextureNameByColor(Color col)
{
    if (col == Cpot1) return "PodPinkMat";
    if (col == Cpot2) return "PodBlueMat";
    if (col == Cpot3) return "PodGreenMat";
    if (col == Cpot4) return "PodYellowMat";
    if (col == BLAND_COLOR) return "PoppyBaseMat";
    return "";
}