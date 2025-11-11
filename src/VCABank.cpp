#include "plugin.hpp"


struct VCABank : Module {
	enum ParamId {
		GAIN_1,
		GAIN_2,
		PARAMS_LEN
	};
	enum InputId {
		INPUT_1,
		INPUT_2,
		INPUTS_LEN
	};
	enum OutputId {
		OUTPUT_1,
		OUTPUT_2,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	VCABank() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
	}

	void process(const ProcessArgs& args) override {
	}
};


struct VCABankWidget : ModuleWidget {
	VCABankWidget(VCABank* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/VCABank.svg")));


		// offset rows by 10mm
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20, 21.38471)), module, VCABank::GAIN_1));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20, 31.38471)), module, VCABank::GAIN_2));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.0435362, 19.797209)), module, VCABank::INPUT_1));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.0435362, 29.797209)), module, VCABank::INPUT_2));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(32.972713, 19.797209)), module, VCABank::OUTPUT_1));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(32.972713, 29.797209)), module, VCABank::OUTPUT_2));



		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
	}
};


Model* modelVCABank = createModel<VCABank, VCABankWidget>("VCABank");