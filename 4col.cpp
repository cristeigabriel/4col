#include <cstdio>
#include "4col.hpp"

// NOTE(para): used to generate enum from config string
// HOWTO if you plan to edit this:
// - create a raw string with your current theme
// - run it through config_entries(split_string_by_newline())
// - take that vector, print entries delimited by ",\n"
// - replace enum between (entry_set_start) and (entry_set_end) in 4col.hpp
// - fix build errors, implement your new entries, otherwise you'll crash!

#if 0
#include <sstream>
std::vector<std::string> split_string_by_newline(std::string&& str)
{
    auto result = std::vector<std::string>{};
    auto ss = std::stringstream{str};
    
    for (std::string line; std::getline(ss, line, '\n');)
        result.push_back(line);
    
    return result;
}

std::vector<std::string> config_entries(std::vector<std::string> &&splitting)
{
    std::vector<std::string> out;
    for (auto &&e : splitting)
    {
        if (e.starts_with(" ") || e.starts_with("//"))
            continue;
        
        if (auto &&ret = e.substr(0, e.find(" ")); !ret.empty())
            out.push_back(std::move(ret));
    }
    
    return out;
}
#endif

namespace wal
{
    // NOTE(para): color stuff
    enum unsigned_color
    {
        rgba,
        argb
    };
    
    struct color
    {
        color(uint8_t r,uint8_t g,uint8_t b,uint8_t a) : r(r),g(g),b(b),a(a) {}
        color(color col,uint8_t a) : r(col.r),g(col.g),b(col.b),a(a) {}
        uint8_t r,g,b,a;
    };
    
    template<unsigned_color type>
        color hex_color(unsigned c)
    {
        if (type == rgba)
            return color((c>>24)&0xff,(c>>16)&0xff, (c>>8)&0xff, c&0xff);
        
        return color((c>>16)&0xff, (c>>8)&0xff, c&0xff, (c>>24)&0xff);
    }
    
    template<unsigned_color type>
        unsigned color_hex(color c)
    {
        if (type == rgba)
            return (c.r<<24)+(c.g<<16)+(c.b<<8)+(c.a);
        
        return (c.a<<24)+(c.r<<16)+(c.g<<8)+(c.b);
    }
    
    // NOTE(para): Q: "Why not use brace initialization?"
    // A: Because in our context it won't work.
    template<class... Args>
        std::vector<unsigned> vargs_to_hex_vector(Args... args)
    {
        std::vector<unsigned> out;
        ((out.push_back(color_hex<argb>(args))), ...);
        return out;
    }
    
    // NOTE(para): logic
    template<unsigned_color type>
        void run(unsigned (&scheme)[16], std::ofstream &output)
    {
        // prepare contents
        struct entry {const char *name; std::vector<unsigned> hex;};
        std::unordered_map<int,entry> data;
        
        // write a helper to declare stuff
#define write_entry(x, ...) data[x] = entry{.name=#x, .hex=vargs_to_hex_vector(__VA_ARGS__)}
#define hc(x) hex_color<type>(x)
#define hca(x,y) color(hex_color<type>(x), y)
        
        // declare stuff
        // NOTE(para): credits for inspiration: https://github.com/dlasagno/vscode-wal-theme/blob/master/src/template.ts
        
        // DEFAULT 4CODER
        
        // status bar
        // background
        write_entry(defcolor_bar, hc(scheme[3]));
        // text
        write_entry(defcolor_base, hc(scheme[7]));
        // prompts
        write_entry(defcolor_pop1, hc(scheme[5]));
        // annotation prompts
        write_entry(defcolor_pop2, hc(scheme[7]));
        // text area background
        write_entry(defcolor_back, hc(scheme[0]));
        // panels
        // inactive
        write_entry(defcolor_margin, hca(scheme[8], 0x33));
        // hover
        write_entry(defcolor_margin_hover, hca(scheme[1], 0x33));
        // active
        write_entry(defcolor_margin_active, hc(scheme[1]));
        // list items
        // inactive
        write_entry(defcolor_list_item, hca(scheme[0], 0x33));
        // hover
        write_entry(defcolor_list_item_hover, hca(scheme[1], 0x33));
        // active
        write_entry(defcolor_list_item_active, hc(scheme[1]));
        // cursor
        // normal, ghost
        write_entry(defcolor_cursor, hc(scheme[5]), hc(scheme[5]));
        // at cursor
        // UPDATE!!!!
        write_entry(defcolor_at_cursor, hc(scheme[0]));
        // cursor underline
        write_entry(defcolor_highlight_cursor_line, hc(scheme[2]));
        // search entry
        // string background
        write_entry(defcolor_highlight, hca(scheme[10], 0x33));
        // string
        write_entry(defcolor_at_highlight, hca(scheme[8], 0x33));
        // mark
        // UPDATE!!!!
        write_entry(defcolor_mark, hc(scheme[0]));
        // text
        // default
        write_entry(defcolor_text_default, hc(scheme[7]));
        // comment
        write_entry(defcolor_comment, hca(scheme[8], 0xb0));
        // comment pops
        write_entry(defcolor_comment_pop, hc(scheme[7]));
        // keyword
        write_entry(defcolor_keyword, hc(scheme[14]));
        // string constant
        write_entry(defcolor_str_constant, hc(scheme[12]));
        // char constant
        write_entry(defcolor_char_constant, hc(scheme[12]));
        // int constant
        write_entry(defcolor_int_constant, hc(scheme[12]));
        // float constant
        write_entry(defcolor_float_constant, hc(scheme[12]));
        // bool constant
        write_entry(defcolor_bool_constant, hc(scheme[12]));
        // preproc
        write_entry(defcolor_preproc, hc(scheme[10]));
        // include
        write_entry(defcolor_include, hc(scheme[10]));
        // special char
        write_entry(defcolor_special_character, hc(scheme[7]));
        // ghost char
        write_entry(defcolor_ghost_character, hc(scheme[3]));
        // highlight junk
        write_entry(defcolor_highlight_junk, hc(scheme[3]));
        // highlight white
        write_entry(defcolor_highlight_white, hca(scheme[7], 0x40));
        // paste
        write_entry(defcolor_paste, hc(scheme[7]));
        // undo
        write_entry(defcolor_undo, hc(scheme[6]));
        // back cycle
        write_entry(defcolor_back_cycle, hc(scheme[0]));
        // text cycle
        // at cursor, next/prev
        write_entry(defcolor_text_cycle, hc(scheme[7]), hc(scheme[7]));
        // line numbers
        // background
        write_entry(defcolor_line_numbers_back, hc(scheme[0]));
        // text
        write_entry(defcolor_line_numbers_text, hc(scheme[1]));
        
        // FLEURY
        
        // cursor
        // inactive
        write_entry(fleury_color_cursor_inactive, hc(scheme[1]));
        // color index
        // product type
        write_entry(fleury_color_index_product_type, hc(scheme[10]));
        // sum type
        write_entry(fleury_color_index_sum_type, hc(scheme[10]));
        // index function
        write_entry(fleury_color_index_function, hc(scheme[13]));
        // constant (enums etc)
        write_entry(fleury_color_index_constant, hc(scheme[15]));
        // macros
        write_entry(fleury_color_index_macro, hc(scheme[14]));
        // 4coder commands
        write_entry(fleury_color_index_4coder_command, hc(scheme[15]));
        // comment tag
        write_entry(fleury_color_index_comment_tag, hc(scheme[8]));
        // declaration
        write_entry(fleury_color_index_decl, hc(scheme[13]));
        // username
        write_entry(fleury_color_comment_user_name, hc(scheme[15]));
        // error
        write_entry(fleury_color_error_annotation, hca(scheme[4], 0x22));
        // warning
        // NOTE(para): this is only in my fleury's layer fork presumably
        write_entry(fleury_color_warning_annotation, hca(scheme[1], 0x22));
        // syntax
        write_entry(fleury_color_syntax_crap, hc(scheme[11]));
        // operators
        write_entry(fleury_color_operators, hc(scheme[11]));
        // inactive panes 
        // darkening overlay
        write_entry(fleury_color_inactive_pane_overlay, hca(scheme[0], 0x44));
        // background
        write_entry(fleury_color_inactive_pane_background, hc(scheme[0]));
        // file progress bar
        write_entry(fleury_color_file_progress_bar, hca(scheme[3], 0x22));
        // braces
        // highlight
        write_entry(fleury_color_brace_highlight, hc(scheme[12]));
        // line
        write_entry(fleury_color_brace_line, hc(scheme[13]));
        // annotation
        write_entry(fleury_color_brace_annotation, hca(scheme[15], 0x60));
        // tokens
        // highlight
        write_entry(fleury_color_token_highlight, hc(scheme[12]));
        // minor highlight
        write_entry(fleury_color_token_minor_highlight, hc(scheme[13]));
        
        // run through data
        
        // utility to write numbers
        auto write_numbers = [](std::ofstream &write, std::vector<unsigned> &&set)
        {
            if (set.size() == 1)
                write<<"0x"<<set[0];
            else 
            {
                write<<'{';
                for (unsigned i = 0;i<set.size()-1;++i)
                    write<<"0x"<<set[i]<<',';
                write<<"0x"<<set[set.size()-1]<<'}';
            }
        };
        
        // all numbers will be hex
        output << std::hex;
        for (int i=entry_set_start+1;i<entry_set_end;++i)
        {
            entry &&gen = std::move(data[i]);
            output<<gen.name<<'=';
            write_numbers(output, std::move(gen.hex));
            output<<";\n";
        }
    }
}


// 0 - gruvbox dark (credits: @dawikur on GitHub)
// 1 - nord (credits: @ada-lovelace on GitHub)
// 2 - your own input scheme
#define COLOR 2

#if COLOR == 0
unsigned scheme[16] = 
{
    0xFF1d2021,
    0xFF3c3836,
    0xFF504945,
    0xFF665c54,
    0xFFbdae93,
    0xFFd5c4a1,
    0xFFebdbb2,
    0xFFfbf1c7,
    0xFFfb4934,
    0xFFfe8019,
    0xFFfabd2f,
    0xFFb8bb26,
    0xFF8ec07c,
    0xFF83a598,
    0xFFd38698,
    0xFFd65d0e,
};
#elif COLOR == 1
unsigned scheme[16] =
{
    0xFF2E3440,
    0xFF3B4252,
    0xFF434C5E,
    0xFF4C566A,
    0xFFD8DEE9,
    0xFFE5E9F0,
    0xFFECEFF4,
    0xFF8FBCBB,
    0xFF88C0D0,
    0xFF81A1C1,
    0xFF5E81AC,
    0xFFBF616A,
    0xFFD08770,
    0xFFEBCB8B,
    0xFFA3BE8C,
    0xFFB48EAD,
};
#elif COLOR == 2
// input here
unsigned scheme[16] = 
{
    0xff000000,
    0xff121212,
    0xff222222,
    0xff333333,
    0xff999999,
    0xffc1c1c1,
    0xff999999,
    0xffc1c1c1,
    0xff5f8787,
    0xffaaaaaa,
    0xff5f81a5,
    0xffd0dfee,
    0xffaaaaaa,
    0xff888888,
    0xff999999,
    0xff444444,
};
#endif

int main()
{
    std::ofstream output(std::filesystem::current_path() / "theme-autogen.4coder", std::ios::out);
    wal::run<wal::argb>(scheme, output);
    output.close();
}