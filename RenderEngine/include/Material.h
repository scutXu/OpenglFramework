#pragma once
#include"Pass.h"
#include<vector>
using std::vector;
class Material
{
public:
	void addPass(PassPtr pass);
	void removePassAt(int i);
	void removePass(PassPtr pass);
	int getPassCount()const;
	PassPtr getPassAt(int i);
private:
	vector<PassPtr> passes;
};
typedef shared_ptr<Material> MaterialPtr;