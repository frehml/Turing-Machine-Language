//
// Created by dries on 12/6/21.
//

#ifndef TO_GROUP_MB_ABSTRACTSYNTAXTREE_H
#define TO_GROUP_MB_ABSTRACTSYNTAXTREE_H

#include "ASTNode.h"

class AbstractSyntaxTree {
public:
    std::vector<ASTNode> nodes;

    const std::vector<ASTNode> &getNodes() const;

    void setNodes(const std::vector<ASTNode> &nodes);

    void addNode(ASTNode &node);

    nlohmann::json toJson();

};



#endif //TO_GROUP_MB_ABSTRACTSYNTAXTREE_H
