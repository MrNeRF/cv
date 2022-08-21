//
// Created by paja on 8/21/22.
//

#include "SceneGraph.h"

namespace scene {
    SceneGraph::SceneGraph() : _rootNode{std::make_unique<Node>()} {
        _rootNode->name = "Root";
    }
    void SceneGraph::InserNode(Node* parent, std::unique_ptr<Node> child) {
        if (parent != nullptr) {
            child->pParent = parent;
            parent->children.push_back(std::move(child));
        } else {
            _rootNode->children.push_back(std::move(child));
        }
    }

    void SceneGraph::RemoveNode(Node* node) {
        ASSERT(node != nullptr)
        if (node == _rootNode.get()) {
            _rootNode = nullptr;
            return;
        } else {
            Node* parent = node->pParent;
            for (auto& spChild : parent->children) {
                if (spChild.get() == node) {
                    spChild = nullptr;
                    return;
                }
            }
        }

        ASSERT(0);
    }

}  // namespace scene