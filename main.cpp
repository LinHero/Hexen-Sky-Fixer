


#include <iostream>
#include <cstdlib>
#include <cstdint>

#include <chrono>
#include <thread>

#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

#include <array>
#include <string>


using s8 = int8_t;
using u8 = uint8_t;

using s16 = int16_t;
using u16 = uint16_t;

using s32 = int32_t;
using u32 = uint32_t;


struct lmpheader{
	u16 width = 0, height = 0;
	u16 offx = 0, offy = 0;
};
struct pixrun{
	u8 offset = 0, length = 0;
	u8 ___PADDING_BYTE1 = 0;
	
	u8 pixels[256] = {};
	u8 ___PADDING_BYTE2 = 0;
};




//==============================================================================//
//		Main Function															//
//==============================================================================//


bool decomp(std::string argv){
	s32 failures = 0;
	std::ifstream infile = {};
	
	while((!infile.is_open()) && (failures++ < 20)){
		std::cout << "CHECK #" << failures << '\n';
		infile.open(argv, std::ios::binary);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	};
	if(failures >= 20){
		std::cout << "FAILED TO ACCESS " << argv << '\n';
		return EXIT_FAILURE;
	};
	
	//		Read the header of pixrun defs
	lmpheader header = {};
	infile.read((char*)&header, sizeof(header));
	
	
	u32 coldats[header.width] = {0};
	infile.read((char*)&coldats, sizeof(u32) * header.width);
	
	//		Clean up the pixruns
	pixrun pixruns[header.width] = {0};
	
	s32 i = 0;
	for(auto& run : pixruns){
		//		Simple prep work
		
		run.offset = 0;
		run.length = header.height;
		
		infile.seekg(coldats[i]);
		
		s32 p = 0;
		while(p < header.height){
			infile.ignore();			//We only need length, not offset
			u8 len = infile.get();

			infile.ignore();			//This is just padding

			infile.read((char*)&run.pixels[p], len);

			infile.ignore();			//This is just padding
			
			p += len;
		}
		i++;
	};
	
	//		Now we need to generate the proper out going coldat table
	
	u32 outcoldat[header.width] = {0};
	u32 outoffset = 8 + (header.width * sizeof(u32));		//Start at the first index
	
	for(auto& out : outcoldat){
		out = outoffset;
		
		//5 comes from the header, minus the pixels, and the FF terminator
		outoffset += 5 + header.height;
	};
	
	//		Finally we output
	std::string name = argv;
	argv.insert(argv.find_last_of('.'), "_FIXED");
	
	
	std::ofstream outfile(argv, std::ios::binary);
	outfile.write((char*)&header, sizeof(u16) * 4);
	
	outfile.write((char*)&outcoldat[0], sizeof(u32) * header.width);
	
	
	//		This requires a bit more manual touch up
	for(auto& run : pixruns){
		outfile.put(0);
		outfile.put(header.height);
		outfile.put(0);
		
		outfile.write((char*)&run.pixels[0], header.height);
		
		outfile.put(0);
		outfile.put(0xFF);
	}
	
	std::cout << name << " CREATED SUCCESSFULLY!" << '\n';
	return EXIT_SUCCESS;
}



s32 main(s32 argc, char** argv){
	if(argc < 2){
		std::cout << "NO FILE GIVEN!" << '\n';
		
	}else if(!fs::exists(argv[1])){
		std::cout << "FILE " << argv[1] << " NOT FOUND!" << '\n';
		
	}else{
		s32 i = 1;
		while(i < argc){
			decomp(argv[i++]);
		}
	}
	
	system("pause");
	return EXIT_SUCCESS;
};