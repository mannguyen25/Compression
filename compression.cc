/******************************************************************************
 *
 * Name:        Man Nguyen
 *
 * Date:        05/12/19
 *
 * Assignment: Compression
 *
 * Description: The file compresses a file with an output of filesize, a huffman
                tree, and a tree path for each character and uncompresses the
                file into the original text as a copy. Huffman encoding is a
                technique for compressing files to save space on storage devices.
                The general idea is to build a full binary tree whose leaves
                represent the unique characters of the file and whose branches
                represent the bits of the code. Compress converts each 8-bit
                binary path into a char, while uncompress reads each char and
                converts it into an 8-bit string. Once it has an 8-bit string,
                it follows path moving a temporary hnode until it reaches a leaf.
                Then, it outputs the character at the leaf until all the characters
                have been outputed to the outstream.
 *
 ******************************************************************************/

#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "minheap.h"

struct hnode {
  int character;
  size_t count;
  hnode *left;
  hnode *right;
  hnode(int character, size_t count,
	      hnode *left=NULL, hnode *right=NULL) :
    character(character), count(count), left(left), right(right) {}
};


int hnode_cmp(hnode * const & a, hnode * const & b)
{
  return a->count < b->count? -1 : a->count > b->count? 1 : a - b;
}

bool show_bits = true;   // might be useful for debugging

std::string byte_to_bits(int byte)
{ /* converts int byte to 8-bit string */
  // Written in class 4/29
  std::string result = "";
  size_t i;
  int v;
  for (i = 0, v = 128; i < 8; i++, v /=2)
    if (byte >= v) {
      result += '1';
      byte -= v;
    }
    else
      result += '0';
  return result;
}


int bits_to_byte(std::string bitstr)
{ /* Converts 8-bit string to int byte*/
	int result = 0;
  size_t i;
  int v;

  // for loop with two values being changed
	for (i = 0, v = 128; i < 8; i++, v /= 2){
		if (bitstr[i] == '1') result += v;
  }
	return result;
}


void get_counts(size_t * counts, size_t & size, std::istream & in)
{
  // POST:  all characters read from in.
  //        counts[k] = the number of occurrences of k in the istream.
  //        size is the number of characters read
  while (true) {
    int ch = in.get();
    if (!in) break;
    // The following line may be deleted or modified.
    // std::cout << static_cast<char>(ch) << std::endl;
    counts[ch]++;
    size++;
  }
}


hnode * make_tree(size_t * counts)
{
  minheap<hnode *> queue(hnode_cmp);
  // Finish.
  // 1. make leaves and insert in  queue
  for (size_t i = 0; i < 256; i++) {
    if (counts[i] != 0)
      queue.add(new hnode(i, counts[i]));
    }

  // 2. Implement tree-building algorithm removing two
  //    trees, combining and reinserting until queue size
  //    is 1.
  while (queue.size() != 1) {

    // get left child hnode
    hnode * left = queue.front();
    queue.remove();

    // get right child hnode
    hnode * right = queue.front();
    queue.remove();

    // combine them
    queue.add(new hnode(left->character, left->count + right->count, left, right));
  }
  // Write code here.

  // 3. Remove and return the resulting tree.
  hnode *result = queue.front();
  queue.remove();
  return result;
}

void make_codes(hnode *tree, std::string *codes, std::string code="")
{
  // codes is an array of 256 strings, initially empty.
  // code is the path so far.
  // You implement:
  // Traverse the tree.
  // code is the path so far.
  // Basic idea:
  // if at a leaf, store path in codes corresponding to value
  // otherwise, recursive call left and right, with updates to code.

  if (!tree) return;
  if (!(tree->left || tree->right)){
    codes[tree->character] = code;
  }
  else{
    make_codes(tree->left, codes, code+="0");
    code.pop_back();
    make_codes(tree->right, codes, code+="1");
  }
}

void write_tree(hnode * tree, std::ostream & ostr)
{
  // Writes the huffman tree onto the output file
  // sample output:  ILaILbLc
  if (tree->left) {
    ostr << 'I';
    write_tree(tree->left, ostr);
    write_tree(tree->right, ostr);
  }
  else ostr << 'L' << static_cast<char>(tree->character);
}

hnode * read_tree(std::istream & istr)
{
  // To be implemented.
  // Inverse of write_tree
  // return NULL;
  char data = istr.get();
  if (data == 'L')
    return new hnode(istr.get(), 0);
  else {
    hnode * left = read_tree(istr);
    hnode * right = read_tree(istr);
    return new hnode(left->character, left->count + right->count, left, right);
  }
}

void write_compress(std::istream & in, std::string *codes, std::ostream & ostr)
{
  /* outputs encoded characters to the ostream */
  std::string output = "";

  // reads characters in file
  while (true){
    int ch = in.get();
    if (!in) break;

    // adds indexed value to output string
    output += codes[ch];

    // if the bistring size is greater than or equal to 8
    // grab the first 8 values, convert to byte, and output the char form
    if (output.length() >= 8)
    {
      ostr << static_cast<char>(bits_to_byte(output.substr(0, 8)));
      output = output.substr(8);
    }
  }
  // if the last encoded paths are less than 8 in bitstring length
  // add 0 until its complete than output the char
  while (output.length() > 0 && output.length() != 8) output += "0";
  ostr << static_cast<char>(bits_to_byte(output));
}

void compress(char *filename)
{
  std::ifstream in(filename);
  size_t counts[256] = { }; // initializes all to zero.
  size_t file_size;
  get_counts(counts, file_size, in);
  in.close();

  if (file_size == 0)
    return;

  hnode * tree = make_tree(counts);

  std::string codes[256];
  make_codes(tree, codes);

  std::cout << file_size;  // for debugging only
  write_tree(tree, std::cout);  // for debugging only

  // Second pass through the input....
  in.open(filename);
  write_compress(in, codes, std::cout);
  in.close();
}


void decode(std::istream &in, std::ostream &ostr, hnode *tree, size_t file_size){
  /* decodes a stream of characters using a Huffman tree */
  std::string decode;
  while (true){
    decode += (char)in.get();
    if (!in) break;
  }

  // counts of the characters output
  size_t count = 0;
  hnode *temp = tree;
  for (size_t i = 0; i < decode.length(); i++) {
    std::string path = byte_to_bits((int)decode[i]);

    // case if somehow the char conversion is negative
    if ((int)decode[i] < 0) path = byte_to_bits(256 + (int)decode[i]);

    // for the length of an 8-bit string
    for (size_t j = 0; j < 8; j++) {

      // when the ostream outputs the correct amount of characters
      // return / exit this for loop
      if (count == file_size) return;

      // if it's 0 make the temporary pointer go left
      if (path[j] == '0' && temp->left) temp = temp->left;

      // if it's 1 make the temporary pointer go right
      else if (path[j] == '1' && temp->right) temp = temp->right;

      // this is the case that we reach a leaf, so output the
      // character and reset the tree
      else {
        ostr << static_cast<char>(temp->character);
        count++;
        temp = tree;

        // since it's in a for loop the leaf case skips
        // the next traversal, therefore it needs to go back 1
        j--;
      }
    }
  }
}


void uncompress()
{
  if (std::cin.peek() == EOF)
    return;  // handle empty file
  size_t file_size;
  std::cin >> file_size;
  hnode *tree = read_tree(std::cin);
  decode(std::cin, std::cout, tree, file_size);

  // FINISH
}

/* DON'T MODIFY BELOW */

bool ends_with(const std::string & str, const std::string & suffix)
{
  return str.size() >= suffix.size() and
    str.substr(str.size()-suffix.size()) == suffix;
}

bool is_compress(const std::string & command)
{
  return ends_with(command, "compress") and
    not ends_with(command, "uncompress");
}

int main(int argc, char **argv)
{
  assert(argc==argc);
  if (is_compress(argv[0]))
    if (std::string(argv[1])=="-b") {
      show_bits = true;
      compress(argv[2]);
    }
    else
      compress(argv[1]);
  else
    uncompress();
}
