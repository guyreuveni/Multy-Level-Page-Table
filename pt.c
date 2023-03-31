#include os.h

int pte_is_valid(uint64_t pte)
{
    uint64_t mask = 1;
    return pte & mask;
}

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn)
{
    uint64_t **curr_node = phys_to_virt(pt);
    uint64_t index, curr_pte, curr_ppn;
    uint64_t mask = 511;

    if (ppn == NO_MAPPING)
    {
        for (int i = 0; i < 5; i++)
        {
            index = mask & vpn;
            vpn = vpn >> 9;

            curr_pte = curr_node[index];
            if (!pte_is_valid(curr_pte))
            {
                break;
            }
            if (i < 4)
            {
                curr_ppn = curr_pte >> 12;
                curr_node = phys_to_virt(cur_ppn);
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
            vpn = vpn >> 9;
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

                curr_node = phys_to_virt(cur_ppn);
                curr_node[index] = ((curr_ppn << 12) | 1);
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
    uint64_t **curr_node = phys_to_virt(pt);
    uint64_t index, curr_pte, curr_ppn;
    uint64_t mask = 511;
    for (int i = 0; i < 5; i++)
    {
        index = mask & vpn;
        vpn = vpn >> 9;

        curr_pte = curr_node[index];
        if (!pte_is_valid(curr_pte))
        {
            return NO_MAPPING;
        }
        curr_ppn = curr_pte >> 12;
        if (i < 4)
        {
            curr_node = phys_to_virt(cur_ppn);
        }
    }

    return curr_ppn;
}