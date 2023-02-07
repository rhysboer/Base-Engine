#pragma once
#include <string>
#include <vector>
#include "NodeElement.h"
#include "imgui.h"

enum class ElementType {
	INT32,
	FLOAT,
	BOOL,
	VECTOR2,
	VECTOR3
};

class INode;

struct Connector {
	Connector(ElementType type) : type(type) 
	{}

	INode* parentNode = nullptr;
	Connector* connectedElement = nullptr;
	ElementType type;
};

class INode
{
	public:
		INode(const std::string& name, const ImVec2& pos) : name(name), windowPos(pos) {
			id = "##" + std::to_string(rand());
		}
		~INode() {}
		
		void DrawNode();
		static void BeginDraw();
		static void EndDraw();

		virtual INode* CreateNode(const ImVec2& position) = 0;
		inline std::string GetName() const { return name; }


	protected:
		std::vector<Connector*> out_elements = std::vector<Connector*>();
		std::vector<Connector*> in_elements = std::vector<Connector*>();

	private:

		void HandleDrag(int index, bool out = true);

		ImVec2 windowPos;
		std::string id;
		std::string name;
		
		static bool isDraggingConnectorOut;
};

