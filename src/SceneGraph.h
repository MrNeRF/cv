//
// Created by paja on 8/21/22.
//

#ifndef CV_SCENEGRAPH_H
#define CV_SCENEGRAPH_H

#include <IRenderable.h>
#include <memory>
#include <vector>

namespace scene {
    struct Node {
        std::string name;
        std::unique_ptr<IRenderable> renderable;
        Node* pParent = nullptr;
        std::vector<std::unique_ptr<Node>> children;
    };

    class SceneGraph {
       public:
        SceneGraph();
        const Node* GetRoot() { return _rootNode.get(); }
        void InserNode(Node* parent, std::unique_ptr<Node> child);
        void RemoveNode(Node* node);

       private:
        std::unique_ptr<Node> _rootNode;
    };

}  // namespace scene

#endif  // CV_SCENEGRAPH_H
