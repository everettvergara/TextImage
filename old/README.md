# TextImage

TextImage is a simple C++17 Compatible Class to handle console buffering. Applications of which ranges from text console games and other software that requires text console animation. I have yet to create a wiki page for this repo.

For reference, the class has the following functions:

```c++
class TextImage {
    public:
        // Constructors, assignments and destructors
        TextImage();
        TextImage(const String &filename);
        TextImage(const String &text, const Color &color, MASK_BIT mask_bit = ON);
        TextImage(const Area &area, MASK_BIT mask_bit = ON);
        TextImage(const Area &area, const Color &color, const Text &text, MASK_BIT = ON);
        TextImage(const TextImage &rhs);
        TextImage(TextImage &&rhs);
        auto operator=(const TextImage &rhs) -> TextImage &; 
        auto operator=(TextImage &&rhs) -> TextImage &;
        ~TextImage() = default;

        // Protected Getters
        auto area() const -> const Area &;
        auto raw_color() -> Uptr_color &;
        auto craw_color() const -> const Uptr_color &;
        auto raw_text() -> Uptr_text &;
        auto craw_text() const -> const Uptr_text &;
        auto raw_mask8bit() -> Uptr_mask8bit &;
        auto craw_mask8bit() const -> const Uptr_mask8bit &;
        auto size_of_mask8bit() -> Sizeof_mask8bit;

        // Text functions
        auto set_text(const Point &point, const Text &text) -> void;
        auto set_text(const Dim &ix, const Text &text) -> void;
        auto get_text(const Point &point) const -> Text;
        auto get_text(const Dim &ix) const -> Text;
        auto show_text() const -> void;
        auto fill_text(const Text &text) -> void;
        auto fill_text(const String &text) -> void;

        // Color functions
        auto set_color(const Point &point, const Color &color) -> void;
        auto set_color(const Dim &ix, const Color &color) -> void;
        auto get_color(const Point &point) const -> Color;
        auto get_color(const Dim &ix) const -> Color;
        auto show_color() const -> void;
        auto fill_color(const Color &color) const -> void;

        // Mask functions
        auto set_mask(const Point &point, MASK_BIT mask_bit) -> void;
        auto set_mask(const Dim &ix, MASK_BIT mask_bit) -> void;
        auto get_mask(const Point &point) const -> MASK_BIT;
        auto get_mask(const Dim &ix) const -> MASK_BIT;
        auto set_all_mask8bit() -> void;  
        auto clear_all_mask8bit() -> void;
        auto create_mask_if_color(const Color &color) -> void;
        auto create_mask_if_text(const Text &text) -> void;
        auto invert_mask() -> void;
        auto show_mask(Dim marker = 0xffff) const -> void;
        auto show_mask_value() const -> void;

        // Text Image functions
        auto get_image(const Rect &rect) const -> TextImage;
        auto put_image(const TextImage &text_image, const Point &point) -> void;
        auto and_image(const TextImage &text_image, const Point &point) -> void;
        auto or_image(const TextImage &text_image, const Point &point) -> void;
        auto xor_image(const TextImage &text_image, const Point &point) -> void;
        auto put_text_color(const String &text, const Color &color, const Point &point) -> void;
        auto show() const -> void;

        // Translate Functions
        // TODO: TEST xlat
        auto xlat_shift_left(Dim shift, TextImageAttribute tia, const Text &default_text = ' ', const Color &default_color = 0, const MASK_BIT &default_mask_bit = OFF) -> void;
        auto xlat_shift_right(Dim shift, TextImageAttribute tia, const Text &default_text = ' ', const Color &default_color = 0, const MASK_BIT &default_mask_bit = OFF) -> void;
        auto xlat_reverse(Dim start, Dim end, TextImageAttribute tia) -> void;
        auto xlat_rotate_left(Dim rotate, TextImageAttribute tia) -> void;
        auto xlat_rotate_right(Dim rotate, TextImageAttribute tia) -> void;
        auto xlat_flip_horizontal(TextImageAttribute tia) -> void;
        auto xlat_flip_vertical(TextImageAttribute tia) -> void;

        // Graphics
        auto gfx_point(const Point &point, const Text &text, const Color &color, const MASK_BIT &mask_bit) -> void;
        
        
        auto gfx_line(const Point &point1, const Point &point2, const Text &text, const Color &color, const MASK_BIT &mask_bit) -> void;
        auto gfx_line_text(const Point &point1, const Point &point2, const Text &text) -> void;
        auto gfx_line_color(const Point &point1, const Point &point2, const Color &color) -> void;
        auto gfx_line_mask(const Point &point1, const Point &point2, const MASK_BIT &mask_bit) -> void;
        
        
        auto gfx_circle(const Point &point, const Dim &radius, const Text &text, const Color &color, const MASK_BIT &mask_bit) -> void;
        auto gfx_circle_text(const Point &point, const Dim &radius, const Text &text) -> void;
        auto gfx_circle_color(const Point &point, const Dim &radius, const Color &color) -> void;
        auto gfx_circle_mask(const Point &point, const Dim &radius, const MASK_BIT &mask_bit) -> void;

        auto gfx_arc(const Point &point, const Dim &radius, const Dim &sa, const Dim &ea, const Text &text, const Color &color, const MASK_BIT &mask_bit) -> void;
        auto gfx_arc_text(const Point &point, const Dim &radius, const Dim &sa, const Dim &ea, const Text &text) -> void;
        auto gfx_arc_color(const Point &point, const Dim &radius, const Dim &sa, const Dim &ea, const Color &color) -> void;
        auto gfx_arc_mask(const Point &point, const Dim &radius, const Dim &sa, const Dim &ea, const MASK_BIT &mask_bit) -> void;
        
        auto gfx_fill_with_text_border(const Point &point, const Text &text) -> void;
        auto gfx_fill_color_border(const Point &point, const Color &color) -> void;
        auto gfx_fill_with_mask_border(const Point &point, const MASK_BIT &mask_bit) -> void;
        
        // Save and Load
        auto save(const String &filename) const -> void;
        auto load(const String &filename) -> void;
        
        // Helper functions
        auto index(const Point &point) const -> Dim;

    protected:
        Area area_{0,0};
        Uptr_color color_{nullptr};
        Uptr_text text_{nullptr};
        Sizeof_mask8bit size_of_mask8bit_{0};
        Uptr_mask8bit mask8bit_{nullptr};

    private:
        auto get_mask8bit_value(const Dim &ix, const Dim &size, const Dim &init_offset = 0) const -> Mask8bitOp;
        auto gfx_circle_loop(const Point &point, const Dim &radius, std::function<void(const Dim)> &tia_set) -> void;
        auto gfx_line_loop(const Point &point1, const Point &point2, std::function<void(const Dim)> &tia_set) -> void;
        auto gfx_arc_loop(const Point &point, const Dim &radius, const Dim &sa, const Dim &ea, std::function<void(const Dim)> &tia_set) -> void;
        auto gfx_fill_loop(const Point &point, std::function<void(const Dim)> &tia_set, std::function<bool(const Dim)> &border_check) -> void;

};

```

Some things you can do with TextImage Class
---

Game of Life Simulation
---
![alt text](https://github.com/everettvergara/TextImage/blob/main/assets/Game%20of%20life.png)

Waving Flag Animation
---
![alt text](https://github.com/everettvergara/TextImage/blob/main/assets/PinoyFlag.png)