#pragma once
#include <memory>
#include <string>

#include "tree.hpp"

class FSNode;
class Folder;
class File;


class FSNode {
 private:
  std::string name;
  TreeNode<FSNode*> *treeNode;


 public:
  FSNode(std::string name);
  virtual ~FSNode() = 0;

  static void ValidateName(std::string name,
                           Folder* parentFolder);
  std::unique_ptr<FSNode> Move();

  std::string GetName() const;
  std::string GetPath() const;
  Folder *ParentFolder() const;
  TreeNode<FSNode*> *GetTreeNode() const;

  void SetName(std::string name);
};

class File : public FSNode {
 private:
  std::string realPath;

 public:
  File(std::string name, std::string realPath);
  std::string GetRealPath() const;
  ~File();
};

class Folder : public FSNode {
 private:
 public:
  Folder(std::string name);
  ~Folder();

  FSNode *Add(FSNode *fsnode);
  FSNode *GetNodeByName(std::string name) const;
  std::unique_ptr<DynamicArraySequence<FSNode*>> GetChildren();
};
