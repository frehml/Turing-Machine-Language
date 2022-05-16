//
// Created by dries on 12/6/21.
//

#include "AbstractSyntaxTree.h"

const std::vector<ASTNode> &AbstractSyntaxTree::getNodes() const {
    return nodes;
}

void AbstractSyntaxTree::setNodes(const std::vector<ASTNode> &nodes) {
    AbstractSyntaxTree::nodes = nodes;
}

void AbstractSyntaxTree::addNode(ASTNode &node) {
    nodes.emplace_back(node);
}

nlohmann::json AbstractSyntaxTree::toJson() {
    nlohmann::json to_export;

    to_export["type"] = "AbstractSyntaxTree";

    for(const auto& it:this->getNodes())
        to_export["children"].emplace_back(it.toJson());

    return to_export;
}
