 //===- llvm/ADT/OwningPtr.h - Smart ptr that owns the pointee ---*- C++ -*-===//
 //
 //                     The LLVM Compiler Infrastructure
 //
 // This file is distributed under the University of Illinois Open Source
 // License. See LICENSE.TXT for details.
 //
 //===----------------------------------------------------------------------===//
 //
 // This file defines and implements the OwningPtr class.
 //
 //===----------------------------------------------------------------------===//

 #ifndef LLVM_ADT_OWNING_PTR_H
 #define LLVM_ADT_OWNING_PTR_H

 #include <cassert>
 #include <cstddef>

 namespace llvm {

 /// OwningPtr smart pointer - OwningPtr mimics a built-in pointer except that it
 /// guarantees deletion of the object pointed to, either on destruction of the
 /// OwningPtr or via an explicit reset().  Once created, ownership of the
 /// pointee object can be taken away from OwningPtr by using the take method.
 template<class T>
 class OwningPtr {
   OwningPtr(OwningPtr const &);             // DO NOT IMPLEMENT
   OwningPtr &operator=(OwningPtr const &);  // DO NOT IMPLEMENT
   T *Ptr;
 public:
   explicit OwningPtr(T *P = nullptr) : Ptr(P) {}

   ~OwningPtr() {
     delete Ptr;
   }

   /// reset - Change the current pointee to the specified pointer.  Note that
   /// calling this with any pointer (including a nullptr) deletes the
   /// current pointer.
   void reset(T *P = nullptr) {
     if (P == Ptr) return;
     T *Tmp = Ptr;
     Ptr = P;
     delete Tmp;
   }

   /// take - Reset the owning pointer to null and return its pointer.  This does
   /// not delete the pointer before returning it.
   T *take() {
     T *Tmp = Ptr;
     Ptr = nullptr;
     return Tmp;
   }

   T &operator*() const {
     assert(Ptr && "Cannot dereference null pointer");
     return *Ptr;
   }

   T *operator->() const { return Ptr; }
   T *get() const { return Ptr; }
   operator bool() const { return Ptr != nullptr; }
   bool operator!() const { return Ptr == nullptr; }

   void swap(OwningPtr &RHS) {
     T *Tmp = RHS.Ptr;
     RHS.Ptr = Ptr;
     Ptr = Tmp;
   }
 };

 template<class T>
 void swap(OwningPtr<T> &a, OwningPtr<T> &b) {
   a.swap(b);
 }

 /// OwningArrayPtr smart pointer - OwningArrayPtr provides the same
 ///  functionality as OwningPtr, except that it works for array types.
 template<class T>
 class OwningArrayPtr {
   OwningArrayPtr(OwningArrayPtr const &);            // DO NOT IMPLEMENT
   OwningArrayPtr &operator=(OwningArrayPtr const &); // DO NOT IMPLEMENT
   T *Ptr;
 public:
   explicit OwningArrayPtr(T *P = nullptr) : Ptr(P) {}

   ~OwningArrayPtr() {
     delete [] Ptr;
   }

   /// reset - Change the current pointee to the specified pointer.  Note that
   /// calling this with any pointer (including a nullptr) deletes the
   /// current pointer.
   void reset(T *P = nullptr) {
     if (P == Ptr) return;
     T *Tmp = Ptr;
     Ptr = P;
     delete [] Tmp;
   }

   /// take - Reset the owning pointer to nullptr and return its pointer.  This does
   /// not delete the pointer before returning it.
   T *take() {
     T *Tmp = Ptr;
     Ptr = nullptr;
     return Tmp;
   }

   T &operator[](std::ptrdiff_t i) const {
     assert(Ptr && "Cannot dereference null pointer");
     return Ptr[i];
   }

   T *get() const { return Ptr; }
   operator bool() const { return Ptr != nullptr; }
   bool operator!() const { return Ptr == nullptr; }

   void swap(OwningArrayPtr &RHS) {
     T *Tmp = RHS.Ptr;
     RHS.Ptr = Ptr;
     Ptr = Tmp;
   }
 };

 template<class T>
 void swap(OwningArrayPtr<T> &a, OwningArrayPtr<T> &b) {
   a.swap(b);
 }


 } // end namespace llvm

 #endif