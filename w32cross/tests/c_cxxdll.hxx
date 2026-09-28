#pragma once

#ifndef ANIMALDECL
#define ANIMALDECL __declspec(dllimport)
#endif

class ANIMALDECL Animal {
public:
	virtual ~Animal();
	void detail();
	virtual void voice() = 0;
};

class ANIMALDECL Cat : public Animal {
public:
	Cat();
	virtual void voice() override;
};

ANIMALDECL Animal * animalFactory();
