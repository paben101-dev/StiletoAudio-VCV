#include "plugin.hpp"
#include <random>
#include <vector>
#include <array>
#include <algorithm>

// TODO:
// 1. Tween output jacks to move around the panel, trippy!
// 2. Shuffle button to shuffle all the outputs around

struct Salvador : Module {
	enum ParamId {
		SPEED_KNOB,

		PARAMS_LEN
	};
	enum InputId {
		INPUTS_LEN
	};
	enum OutputId {
		CLOCKOUTPUT_1,
		CLOCKOUTPUT_2,
		CLOCKOUTPUT_3,
		CLOCKOUTPUT_4,
		CLOCKOUTPUT_5,
		CLOCKOUTPUT_6,
		CLOCKOUTPUT_7,
		CLOCKOUTPUT_8,
		CLOCKOUTPUT_9,
		CLOCKOUTPUT_10,

		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	struct Clock {

		float timeElapsed = 0.f;
		float pulseTimer = 0.f;
		float pulseLength = 0.01f;

		void tick(const ProcessArgs& args, Param& param, Output& output, const float primeNumber) {
    		const float globalTimeScale = 0.25f; // 0.25 = 4× slower; was too fast
			timeElapsed += args.sampleTime*primeNumber*globalTimeScale;

			if (timeElapsed >= param.getValue()) {
				timeElapsed = 0.f;
				pulseTimer = pulseLength;
			}

			if (pulseTimer > 0.f) {
				output.setVoltage(10.f);
				pulseTimer -= args.sampleTime;
			} else {
				output.setVoltage(0.f);
			}
		}
	};
	std::vector<Clock> clocks;
	std::vector<float> basePrimes {1, 2, 3, 5, 7, 11, 13, 17, 19, 23};
	std::vector<float> primesWithVariance = basePrimes; // start same

	struct SmoothRandom {

    	float current = 0.f;
    	float target  = 0.f;
    	int updateCounter = 0;
    	int updateRate = 4800;
    	float smoothness = 0.05f;

		float operator()(const ProcessArgs& args) {
			if (++updateCounter >= updateRate) {
				updateCounter = 0;
				target = random::uniform() * 2.f - 1.f;
			}
			float alpha = args.sampleTime / (smoothness + args.sampleTime);
			current += alpha * (target - current);
			return current;
    	}
}; std::array<SmoothRandom, 10> randoms;


	Salvador() {

		for (int i = 0; i < 10; ++i)
    		clocks.emplace_back(); // constructs 10 Clock objects
	
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

		configParam(SPEED_KNOB, 5.f, 0.1, 1.f, "Clock Rate");

		for (int i = 0; i < OUTPUTS_LEN; ++i) {
			configOutput(i, "Clock Output");
		}
		
	}

void process(const ProcessArgs& args) override {

	const float driftAmount = 0.5f; // ±50% around the base
	
		// Smoothly vary primes using randoms
		for (size_t i = 0; i < primesWithVariance.size(); ++i) {
			primesWithVariance[i] = basePrimes[i] * (1.f + randoms[i](args) * driftAmount);
		}
		
		for (int i = 0; i < OUTPUTS_LEN; ++i) {
			clocks[i].tick(args, params[SPEED_KNOB], outputs[i], primesWithVariance[i]);
		}
	}
};

struct PlusMinusWithVariance {
	std::random_device rnd;
    std::mt19937 gen;
	std::uniform_real_distribution<float> dist;

    PlusMinusWithVariance() : gen(rand()), dist(-5.f, 5.f) {}

    float operator()(int a) {
        return a + dist(gen);
    }
};

struct SalvadorWidget : ModuleWidget {

PlusMinusWithVariance plusMinusWithVariance;

	SalvadorWidget(Salvador* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Salvador.svg")));
		
	addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(34, 6)), module, Salvador::SPEED_KNOB));

	// makes the outputs in new starting positions every time using PlusMinusWithVariance functor
	addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(plusMinusWithVariance(9), plusMinusWithVariance(19))), module, Salvador::CLOCKOUTPUT_1));
	addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(plusMinusWithVariance(30), plusMinusWithVariance(27))), module, Salvador::CLOCKOUTPUT_2));
	addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(plusMinusWithVariance(22), plusMinusWithVariance(37))), module, Salvador::CLOCKOUTPUT_3));
	addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(plusMinusWithVariance(8), plusMinusWithVariance(48))), module, Salvador::CLOCKOUTPUT_4));
	addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(plusMinusWithVariance(23), plusMinusWithVariance(58))), module, Salvador::CLOCKOUTPUT_5));
	addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(plusMinusWithVariance(15), plusMinusWithVariance(78))), module, Salvador::CLOCKOUTPUT_6));
	addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(plusMinusWithVariance(31), plusMinusWithVariance(79))), module, Salvador::CLOCKOUTPUT_7));
	addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(plusMinusWithVariance(12), plusMinusWithVariance(93))), module, Salvador::CLOCKOUTPUT_8));
	addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(plusMinusWithVariance(35), plusMinusWithVariance(101))), module, Salvador::CLOCKOUTPUT_9));
	addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(plusMinusWithVariance(21), plusMinusWithVariance(112))), module, Salvador::CLOCKOUTPUT_10));

	}
};


Model* modelSalvador = createModel<Salvador, SalvadorWidget>("Salvador");