#include "mc/region.hpp"
#include "mc/utils.hpp"

#include "nbt/nbt.hpp"

#include <list>

#include <boost/foreach.hpp>
#include <iostream>
#include <iomanip>

using namespace std;

struct inspect_context {
  int width;
};

void begin_compound(inspect_context* inspect, nbt::String name) {
  cout << setw(inspect->width) << ""
       << "BEGIN Compound(" << name << ")" << endl;
  inspect->width += 2;
}

void end_compound(inspect_context* inspect, nbt::String name) {
  inspect->width -= 2;
  cout << setw(inspect->width) << ""
       << "END Compound(" << name << ")" << endl;
}

void begin_list(inspect_context* inspect, nbt::String name, nbt::Byte type, nbt::Int length) {
  cout << setw(inspect->width) << ""
       << "BEGIN List(" << name << ", " << nbt::tag_string_map[type] << ", " << length << ")" << endl;
  inspect->width += 2;
}

void end_list(inspect_context* inspect, nbt::String name) {
  inspect->width -= 2;
  cout << setw(inspect->width) << ""
       << "END List(" << name << ")" << endl;
}

void register_long(inspect_context* inspect, nbt::String name, nbt::Long value) {
  cout << setw(inspect->width) << ""
       << "Long(" << name << "): " << dec << value << endl;
}

void register_short(inspect_context* inspect, nbt::String name, nbt::Short value) {
  cout << setw(inspect->width) << ""
       << "Short(" << name << "): " << value << endl;
}

void register_string(inspect_context* inspect, nbt::String name, nbt::String value) {
  cout << setw(inspect->width) << ""
       << "String(" << name << "): " << value << endl;
}

void register_float(inspect_context* inspect, nbt::String name, nbt::Float value) {
  cout << setw(inspect->width) << ""
       << "Float(" << name << "): " << value << endl;
}

void register_double(inspect_context* inspect, nbt::String name, nbt::Double value) {
  cout << setw(inspect->width) << ""
       << "Double(" << name << "): " << value << endl;
}

void register_int(inspect_context* inspect, nbt::String name, nbt::Int value) {
  cout << setw(inspect->width) << ""
       << "Int(" << name << "): " << dec << value << endl;
}

void register_byte(inspect_context* inspect, nbt::String name, nbt::Byte value) {
  cout << setw(inspect->width) << ""
       << "Byte(" << name << "): 0x" << hex << (int)value << endl;
}

void register_byte_array(inspect_context* inspect, nbt::String name, nbt::ByteArray* value) {
  cout << setw(inspect->width) << ""
       << "ByteArray(" << name << "): " << "(" << int(value->length) << "B binary)" << endl;
  delete value;
}

int main(int argc, char* argv[]) {
  using mc::utils::level_coord;

  if (argc < 2) {
    return 1;
  }

  inspect_context ctx;
  
  nbt::Parser<inspect_context> parser(&ctx);
  parser.begin_compound = begin_compound;
  parser.end_compound = end_compound;
  parser.begin_list = begin_list;
  parser.end_list = end_list;
  parser.register_long = register_long;
  parser.register_short = register_short;
  parser.register_string = register_string;
  parser.register_float = register_float;
  parser.register_double = register_double;
  parser.register_int = register_int;
  parser.register_byte = register_byte;
  parser.register_byte_array = register_byte_array;

  mc::region region(argv[1]);

  list<level_coord> coords;

  region.read_coords(coords);

  BOOST_FOREACH(level_coord c, coords) {
    std::stringstream oss;
    region.read_data(c.get_x(), c.get_z(), oss);
    std::string chunk = oss.str();
    ctx.width = 0;
    std::cout << "size = " << chunk.size() << std::endl;
    parser.parse_buffer(chunk.c_str(), chunk.size());
  }
}