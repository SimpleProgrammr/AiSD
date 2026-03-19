#pragma once

class GridMap {
    private:
        long _height;
        long _width;

        struct _row {
            long _r_width;
            unsigned char * cells;
            unsigned char& operator[] (long index) const {return cells[index];};
            void _fill_row_with(unsigned char val) const {
                std::fill_n(cells,_r_width,val);
            }

            _row() {
                _r_width = 0;
                cells = nullptr;
            }

            _row(long width) {
                _r_width = width;
                cells = new unsigned char[_r_width];
            }
        };

        struct _row * _data;



    public:
        GridMap(long height, long width, unsigned char fill);

        [[nodiscard]] long getHeight() const;
        [[nodiscard]] long getWidth() const;

        void _fill_grid_with(unsigned char val) const;
        unsigned char operator()(long height, long width) const ;
        _row operator[](long height) const ;

};