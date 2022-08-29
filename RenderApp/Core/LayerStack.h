#pragma once

#include "Events/Event.h"
#include "Layer.h"

namespace VEE {

	class LayerStack {
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverLay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	
	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}