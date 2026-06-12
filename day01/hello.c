#include <stdio.h>

int main(void) {
    double Kc = 800.0;  /* cutting coefficient  [N/mm²] */
    double Ke = 15.0;   /* edge coefficient     [N/mm]  */
    double b  = 1.0;    /* chip width           [mm]    */
    double h  = 0.057;  /* chip thickness       [mm]    */

    double F_shearing  = Kc * b * h;
    double F_ploughing = Ke * b;
    double F_total     = F_shearing + F_ploughing;

    printf("Shearing  : %8.4f N\n", F_shearing);
    printf("Ploughing : %8.4f N\n", F_ploughing);
    printf("Total     : %8.4f N\n", F_total);

    /* Physics gate — limiting case */
    printf("h=0 check : %8.4f N  (must equal Ke*b = %.4f)\n",
           Kc*b*0.0 + Ke*b, Ke*b);

    return 0;
}
