#include "INode.h"
#include "Resources.h"

#include "Gizmos.h"
#include "Logging.h"

#define WINDOW_PADDING_VERTICAL 10.0f
#define FRAME_PADDING 6.0f
#define SPRITE_SIZE 16

bool INode::isDraggingConnectorOut = true;

void INode::DrawNode()
{
	ImGui::Begin((name + id).c_str(), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	// Draw In Elements
	for (int i = 0; i < in_elements.size(); i++) {
		ImGui::Image((void*)Resources::GetCircle()->GetID(), ImVec2(SPRITE_SIZE, SPRITE_SIZE));
		HandleDrag(i, false);
		ImGui::SameLine();
		ImGui::Text("In");
	}

	ImVec2 pos;

	// Draw Out Elements
	for (int i = 0; i < out_elements.size(); i++) {
		pos.x = 150.0f;
		pos.y = 29.0f + (i * 20.0f);
		ImGui::SetCursorPos(pos);

		ImGui::Text("Out");
		ImGui::SameLine();
		ImGui::Image((void*)Resources::GetCircle()->GetID(), ImVec2(SPRITE_SIZE, SPRITE_SIZE));
		HandleDrag(i, true);
	}

	ImGui::End();

	// Draw Out Connectors
}

void INode::HandleDrag(int index, bool out)
{
	const float fontSize = ImGui::GetFontSize();
	const float framePadding = fontSize + FRAME_PADDING + WINDOW_PADDING_VERTICAL;

	Connector* connector = out ? out_elements[index] : in_elements[index];

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoPreviewTooltip | ImGuiDragDropFlags_SourceAllowNullID)) {
		ImGui::SetDragDropPayload("Payload", connector, sizeof(Connector*));

		isDraggingConnectorOut = out;

		ImVec2 cursorPos = ImGui::GetWindowPos();
		cursorPos.x += ImGui::GetWindowSize().x * out;
		cursorPos.y += framePadding + (index * 20.0f) + (SPRITE_SIZE / 2.0f);
		ImGui::GetBackgroundDrawList()->AddLine(cursorPos, ImGui::GetMousePos(), IM_COL32(200, 0, 200, 255), 2.0f);
		ImGui::EndDragDropSource();
	}
	else if (ImGui::BeginDragDropTarget() && isDraggingConnectorOut != out) 
	{
		Connector* payload = (Connector*)ImGui::AcceptDragDropPayload("Payload");

		if (payload) {
			BE::Logging::Log("ASS");
		}

		ImGui::EndDragDropTarget();
	}

	
}


void INode::BeginDraw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, WINDOW_PADDING_VERTICAL));
}

void INode::EndDraw()
{
	ImGui::PopStyleVar();
}

