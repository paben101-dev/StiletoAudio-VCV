#include "plugin.hpp"

extern Model* modelSineBank;
extern Model* modelVCABank;
extern Model* modelSalvador;

Plugin* pluginInstance;


void init(Plugin* p) {
	pluginInstance = p;

	// Add modules here
	p->addModel(modelSineBank);
	p->addModel(modelVCABank);
	p->addModel(modelSalvador);

	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when your module is created to reduce startup times of Rack.
}
