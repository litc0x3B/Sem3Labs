#include "virtualFS.hpp"

#include <unistd.h>

#include <memory>
#include <stdexcept>

#include "sequence/dynamicArraySequence.hpp"
#include "tree.hpp"

///////////////////////////////////////FSnode/////////////////////////////////////
FSNode::FSNode(std::string name)
{
  treeNode = new TreeNode<FSNode *>(this);
  SetName(name);
}

FSNode::~FSNode()
{
  for (int i = 0; i < treeNode->Children()->GetSize(); i++)
  {
    delete treeNode->Children()->At(i)->Value();
    treeNode->Children()->Delete(i);
  }

  delete treeNode;
}

void FSNode::ValidateName(std::string name, Folder *parentFolder)
{
  if (parentFolder != nullptr)
  {
    if (parentFolder->GetNodeByName(name) != NULL)
    {
      throw std::runtime_error("two files with the same testName in the folder");
    }
  }

  if (name == "")
  {
    throw std::runtime_error("empty file testName");
  }

  if (name.find('/') != std::string::npos || name == ".."){
    throw std::runtime_error("invalid file testName");
  }
}

std::string FSNode::GetPath() const
{
  if (ParentFolder() == nullptr)
  {
    return "/" + GetName();
  }

  return ParentFolder()->GetPath() + "/" + GetName();
}


std::unique_ptr<FSNode> FSNode::Move()
{
  treeNode->Extract();
  return std::move(std::unique_ptr<FSNode>(this));
}

std::string FSNode::GetName() const { return name; }
TreeNode<FSNode *> *FSNode::GetTreeNode() const { return treeNode; }

Folder *FSNode::ParentFolder() const
{
  if (treeNode->Parent() != nullptr)
  {
    return dynamic_cast<Folder *>(treeNode->Parent()->Value());
  }
  return nullptr;
}

void FSNode::SetName(std::string name)
{
  ValidateName(name, ParentFolder());
  this->name = name;
}

/////////////////////////////////////File/////////////////////////////////////
File::File(std::string name, std::string realPath) : FSNode(name), realPath(realPath) {}

std::string File::GetRealPath() const { return realPath; }

File::~File(){};

/////////////////////////////////////Folder/////////////////////////////////////
Folder::Folder(std::string name) : FSNode(name) {}
Folder::~Folder() {}

FSNode *Folder::GetNodeByName(std::string name) const
{
  return GetTreeNode()->FindChild([name](FSNode *node) { return node->GetName() == name; });
}

FSNode *Folder::Add(FSNode *fsnode)
{
  if (fsnode == nullptr)
  {
    throw std::runtime_error("fsnode equals nullptr");
  }
  ValidateName(fsnode->GetName(), this);
  GetTreeNode()->Add(fsnode->GetTreeNode());
  return fsnode;
}

std::unique_ptr<DynamicArraySequence<FSNode *>> Folder::GetChildren()
{
  int size = GetTreeNode()->Children()->GetSize();
  auto arr = new DynamicArraySequence<FSNode *>();

  for (int i = 0; i < size; i++)
  {
    arr->Append(GetTreeNode()->Children()->At(i)->Value());
  }

  return std::move(std::unique_ptr<DynamicArraySequence<FSNode *>>(arr));
}
