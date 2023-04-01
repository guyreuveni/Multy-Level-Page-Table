#include "os.h"

int pte_is_valid(uint64_t pte)
{
    uint64_t mask = 1;
    return pte & mask;
}

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn)
{
    uint64_t *curr_node = phys_to_virt(pt << 12);
    uint64_t index, curr_pte, curr_ppn;
    uint64_t mask = (uint64_t)511;
    mask = mask << 36;
    int i;

    if (ppn == NO_MAPPING)
    {
        for (i = 0; i < 5; i++)
        {
            index = mask & vpn;
            index = index >> 36;
            vpn = vpn << 9;

            curr_pte = curr_node[index];
            if (!pte_is_valid(curr_pte))
            {
                break;
            }
            if (i < 4)
            {
                curr_node = phys_to_virt(curr_pte - 1);
            }
            else
            {
                curr_node[index] = 0;
            }
        }
    }

    else
    {
        for (i = 0; i < 5; i++)
        {
            index = mask & vpn;
            index = index >> 36;
            vpn = vpn << 9;
            curr_pte = curr_node[index];

            if (i < 4)
            {
                if (!pte_is_valid(curr_pte))
                {
                    curr_ppn = alloc_page_frame();
                }
                else
                {
                    curr_ppn = curr_pte >> 12;
                }
                curr_node[index] = ((curr_ppn << 12) | 1);
                curr_node = phys_to_virt(curr_ppn << 12);
            }
            else
            {
                curr_node[index] = ((ppn << 12) | 1);
            }
        }
    }
}

uint64_t page_table_query(uint64_t pt, uint64_t vpn)
{
    uint64_t *curr_node = phys_to_virt(pt << 12);
    uint64_t index, curr_pte;
    uint64_t mask = (uint64_t)511;
    mask = mask << 36;
    int i;

    for (i = 0; i < 5; i++)
    {
        index = mask & vpn;
        index = index >> 36;
        vpn = vpn << 9;

        curr_pte = curr_node[index];
        if (!pte_is_valid(curr_pte))
        {
            return NO_MAPPING;
        }
        if (i < 4)
        {
            curr_node = phys_to_virt(curr_pte - 1);
        }
    }

    return (curr_pte >> 12);
}
