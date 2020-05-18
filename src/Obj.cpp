//
// Created by denis on 22.08.19.
//

#include "Obj.hpp"

Obj::o_alloc Obj::alloc = Obj::o_alloc();

Obj::Obj(int type, Obj *par, Obj *right, Obj *left) : type_(type), parent_(par), right_(right), left_(left){}

Obj* Obj::operator()(Obj *left, Obj *right) {
    //Work with left child

    left_ = left;
    if(left)
        left->set_parent(this);
    //Work with right child
    right_ = right;
    if(right)
        right->set_parent(this);

    return this;
}

void Obj::set_parent(Obj *par) {
    parent_ = par;
}

void Obj::set_left(Obj* left){
    left_ = left;
}

void Obj::set_right(Obj *right) {
    right_ = right;
}

int Obj::get_type() const {
    return type_;
}

Obj* Obj::get_left() const {
    return left_;
}

Obj* Obj::get_right()const{
    return right_;
}

Obj* Obj::get_parent() const {
    return parent_;
}

Obj* Obj::create(int type)const {
    Obj* ptr = o_traits::allocate(Obj::alloc, 1);
    o_traits::construct(Obj::alloc, ptr, type, nullptr, nullptr, nullptr);
    return ptr;
}

void Obj::remove(Obj* root) {

    if(!root)
        return;

    if (root->get_right() == nullptr && root->get_left() == nullptr) {
        Obj::o_traits::destroy(Obj::alloc, root);
        Obj::o_traits::deallocate(Obj::alloc, root, 1);
        return;
    }

    if(!(root->get_left() == nullptr && root->get_right() != nullptr))
        remove(root->get_left());

    remove(root->get_right());


    Obj::o_traits::destroy(Obj::alloc, root);
    Obj::o_traits::deallocate(Obj::alloc, root, 1);
}

void Obj::MakeChild(Obj *child) {
    if(this == this->get_parent()->get_left())
        this->get_parent()->set_left(child);
    else
        this->get_parent()->set_right(child);
}