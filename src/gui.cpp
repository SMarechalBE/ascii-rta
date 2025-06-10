#include "gui.h"
#include <ncurses.h>
#include <string>

Gui::Gui()
{
    initscr();
    noecho();
    curs_set(0);
    timeout(100); // Non-blocking getch
}

Gui::~Gui() { endwin(); }

void Gui::draw(const std::array<float, 10>& octave_bands)
{
    erase();

    // Help line
    mvprintw(0, 0, "Press q to quit | Toggle sources: 1=Sine  2=Pink  3=Mic");

    // Number of rows for max bar height
    static constexpr auto max_height = 20;

    // Values are mapped in the dB model [-100, 0]
    static constexpr auto min_value = -100.0f;
    static constexpr auto max_value = 0.0f;

    static constexpr auto bar_width = 7;
    static constexpr auto y_axis_offset = 8;
    static constexpr auto chart_width = bar_width * 10;
    static constexpr auto total_width = y_axis_offset + chart_width;

    // Draw Y-axis labels on both sides
    for (auto db = 0; db >= -90; db -= 10)
    {
        int y = 1 + static_cast<int>((max_value - db) / (max_value - min_value) * max_height);
        mvprintw(y, 0, "%4d|", db); // Left side
        mvprintw(y + 1, 4, "|"); // Left side empty space
        mvprintw(y, total_width - 5, "|%4d", db); // Right side
        mvprintw(y + 1, total_width - 5, "|"); // Right side empty space
    }

    for (auto octave_idx = 0; octave_idx < octave_bands.size(); ++octave_idx)
    {
        const auto clamped_value = std::min(std::max(octave_bands[octave_idx], min_value), max_value);

        // Draw bars using clamped value
        const auto height = static_cast<int>((clamped_value - min_value) / (max_value - min_value) * max_height);
        const int x = octave_idx * 7 + y_axis_offset;
        for (auto y = 0; y < height; ++y)
        {
            mvprintw(max_height - y, x, "#");
        }

        // Draw band number or frequency label below bars
        static const std::array<std::string, 10> octave_frequencies{"31.5Hz",
                                                                    "63Hz",
                                                                    "125Hz",
                                                                    "250Hz",
                                                                    "500Hz",
                                                                    "1KHz",
                                                                    "2KHz",
                                                                    "4KHz",
                                                                    "8KHz",
                                                                    "16KHz"};
        mvprintw(max_height + 1, x - 2, "%s", octave_frequencies[octave_idx].c_str());
    }

    mvprintw(max_height + 3,
             0,
             "Sources: [Sine: %s] [Pink: %s] [Mic: %s]",
             sine_on_ ? "ON" : "OFF",
             pink_on_ ? "ON" : "OFF",
             mic_on_ ? "ON" : "OFF");

    refresh();
}
