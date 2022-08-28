//
// Created by paja on 8/21/22.
//

#include "SceneGraph.h"

namespace scene {
    static Node* FindNode(Node* pNode, const std::string& rNodeName) {
        if (pNode == nullptr) {
            return nullptr;
        }
        if (pNode->name == rNodeName) {
            return pNode;
        } else if (pNode->children.size() == 0) {
            return nullptr;
        } else {
            for (auto& spChild : pNode->children) {
                Node* pResult = FindNode(spChild.get(), rNodeName);
                if (pResult != nullptr && pResult->name == rNodeName) {
                    return pResult;
                }
            }
        }

        return nullptr;
    }

    SceneGraph::SceneGraph() : _rootNode{std::make_unique<Node>()} {
        _rootNode->name = "Root";
    }
    Node* SceneGraph::InserNode(Node* parent, std::unique_ptr<Node> child) {
        if (parent != nullptr) {
            child->pParent = parent;
            parent->children.push_back(std::move(child));
            return parent->children.back().get();
        } else {
            _rootNode->children.push_back(std::move(child));
            return _rootNode->children.back().get();
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

    Node* SceneGraph::GetNode(const std::string& rNodeName) {
        if (_rootNode == nullptr) {
            ASSERT(0)  // Something went most likely wrong. Please debug.
            return nullptr;
        }
        return FindNode(_rootNode.get(), rNodeName);
    }

}  // namespace scene