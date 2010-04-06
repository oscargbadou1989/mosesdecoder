// $Id: mempool.h 87 2007-09-10 23:17:43Z hieu $

/******************************************************************************
 IrstLM: IRST Language Model Toolkit
 Copyright (C) 2006 Marcello Federico, ITC-irst Trento, Italy

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA

******************************************************************************/

// An efficient memory manager 
// by M. Federico 
// Copyright Marcello Federico, ITC-irst, 1998

#ifndef MF_MEMPOOL_H
#define MF_MEMPOOL_H

#ifndef NULL
const int NULL=0;
#endif

#include <iostream>  // std::ostream

//! Memory block
/*! This can be used by:
- mempool to store items of fixed size
- strstack to store strings of variable size
*/

class memnode{
  friend class mempool;   //!< grant access 
  friend class strstack;  //!< grant access 
  char          *block;   //!< block of memory
  memnode        *next;   //!< next block ptr    
};


//! Memory pool

/*! A memory pool is composed of:
   - a linked list of blocknum memory blocks
   - each block might contain up to block_size items
   - each item is made of exactly item_size bytes
*/

class mempool{
  int         block_size;         //!< number of entries per block
  int         item_size;          //!< number of bytes per entry
  int         true_size;          //!< number of bytes per block
  memnode*   block_list;          //!< list of blocks
  char*       free_list;          //!< free entry list
  int         entries;            //!< number of stored entries  
  int         blocknum;           //!< number of allocated blocks  
 public:

  //! Creates a memory pool
  mempool(int is, int bs); 

  //! Destroys memory pool
  ~mempool();
  
  //! Prints a map of memory occupancy
  void map(std::ostream& co);

  //! Allocates a single memory entry
  char *allocate();

  //! Frees a single memory entry
  int free(char* addr);

  //! Prints statistics about this mempool 
  void stat();

  //! Returns effectively used memory (bytes) 
  /*! includes 8 bytes required by each call of new */
     
  int used(){return blocknum * (true_size + 8);}; 
  
  //! Returns amount of wasted memory (bytes) 
  int wasted(){return used()-(entries * item_size);};
}; 

//! A stack to store strings 

/*!
  The stack is composed of 
  - a list of blocks memnode of fixed size
  - attribute blocknum tells the block on top
  - attribute idx tells position of the top string
*/

class strstack{
  memnode* list; //!< list of memory blocks
  size_t   size;    //!< size of each block
  size_t    idx;    //!< index of last stored string
  size_t  waste;    //!< current waste of memory
  size_t memory;    //!< current use of memory
  int entries;   //!< current number of stored strings
  int blocknum;  //!< current number of used blocks

 public:

  strstack(size_t bs=1000);

  ~strstack();
  
  char *push(char *s);
  
  char *pop();
  
  char *top();
  
  void stat();
  
  size_t used(){return memory;}
  
  size_t wasted(){return waste;}
};

//! Manage multiple memory pools 

/*!
  This class permits to manage memory pools 
  with items up to a specified size.
  - items within the allowed range are stored in memory pools
  - items larger than the limit are allocated with new
*/

class storage{
  mempool **poolset;  //!< array of memory pools
  int setsize;        //!< number of memory pools/maximum elem size 
  int poolsize;       //!< size of each block
  int newmemory;      //!< stores amount of used memory
  int newcalls;       //!< stores number of allocated blocks
 public:

  //! Creates storage
  storage(int maxsize,int blocksize);

  //! Destroys storage
  ~storage();

  //! Allocates memory 
  char *allocate(int size);

  //! Realloc memory
  char *reallocate(char *oldptr,int oldsize,int newsize);

  //! Frees memory of an entry   
  int free(char *addr,int size=0);

  //! Prints statistics about storage
  void stat();
};

#endif