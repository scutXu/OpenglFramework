#include"../RenderEngine/include/Material.h"
void Material::addPass(PassPtr pass)
{
	passes.push_back(pass);
}
void Material::removePassAt(int i)
{
	auto iter = passes.begin() + i;
	passes.erase(iter);
}
void Material::removePass(PassPtr pass)
{
	for (auto iter = passes.begin(); iter != passes.end(); ++iter) {
		if (*iter == pass) {
			passes.erase(iter);
			return;
		}
	}
}
int Material::getPassCount()const
{
	return passes.size();
}
PassPtr Material::getPassAt(int i)
{
	return *(passes.begin() + i);
}
