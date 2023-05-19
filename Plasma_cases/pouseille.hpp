#pragma once

#include "Plasma.hpp"
#include "../Variable.hpp"

/*
To Make your own plasma class excecute the following steps
- Pick you class name and replace every instance of the term PlasmaTemplate in both the header and cpp file
*/

class Pouseille : public Plasma
{
public:
	//Rename Constructor and destructor methods
	Pouseille() {};
	~Pouseille() = default;
private:
	//-----Over ride functions-----
	//These functions must be built for every derived plasma class
	void pickVariables() override;

	void seta_fns() override;

	void setKappas() override;
	void setdudKappas() override;
	void setdqdKappas() override;

	void setSources() override;
	void setdudSources() override;
	void setdqdSources() override;
	void setdsigdSources() override;
	//-----------------------------

	double nu = 1.0;
	double G = 1.0;
};