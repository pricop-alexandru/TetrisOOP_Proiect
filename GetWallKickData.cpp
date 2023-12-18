
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
//
// Created by Alex Pricop on 11/3/2023.
//
#include <vector>

#include "Global.hpp"
#include "GetWallKickData.hpp"

std::vector<Position> get_wall_kick_data(bool i_is_i_shape, unsigned char i_current_rotation, unsigned char i_next_rotation)
{
    //site-ul are y-ul inversat, de asemenea am nevoie de rotirea actuala si cea viitoare
    if (0 == i_is_i_shape)
    {
        switch (i_current_rotation)
        {
            case 0:
            case 2:
            {
                switch (i_next_rotation)
                {
                    case 1:
                    {
                        return {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}};
                    }
                    case 3:
                    {
                        return {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}};
                    }
                }
                [[fallthrough]];
            }
            case 1:
            {
                return {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}};
            }
            case 3:
            {
                return {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}};
            }
        }
        return {{0, 0}};
    }
    else
    {
        switch (i_current_rotation)
        {
            case 0:
            {
                switch (i_next_rotation)
                {
                    case 1:
                    {
                        return {{0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}};
                    }
                    case 3:
                    {
                        return {{0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}};
                    }
                }
                [[fallthrough]];
            }
            case 1:
            {
                switch (i_next_rotation)
                {
                    case 0:
                    {
                        return {{0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2}};
                    }
                    case 2:
                    {
                        return {{0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}};
                    }
                }
                [[fallthrough]];
            }
            case 2:
            {
                switch (i_next_rotation)
                {
                    case 1:
                    {
                        return {{0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}};
                    }
                    case 3:
                    {
                        return {{0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2}};
                    }
                }
                [[fallthrough]];
            }
            case 3:
            {
                switch (i_next_rotation)
                {
                    case 0:
                    {
                        return {{0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}};
                    }
                    case 2:
                    {
                        return {{0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}};
                    }
                }
                [[fallthrough]];
                default:
                    break;
            }
        }
        return {{0, 0}};
    }
}
#pragma GCC diagnostic pop