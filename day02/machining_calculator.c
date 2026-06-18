/*  machining_calculator.c - Day 2 practice, amrc-prep
    Calucates spindle speed, table feed, and MRR from basic cutting parameter
    Mirros the calculations as AMRC engineer makes before setting uo a trial.
    
    Compile: gcc -Wall -Wextra -std=c11 machining_calulator.c -o machining_calculator -lm
    Run:     ./machining_calculator
*/

#include <stdio.h>
#include <math.h>       /* for M_PI - requires -lm at link time*/

/* Deine PI exlicitly as a fallback if M_PI not available */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* --------------------- Function declarations--------------------------*/

/*  calc_spindle_speed: N[RPM] = (Vc [m/min] x 1000) / (pi x D [mm]) 
    Returns -1.0 and prints error if inputs are invalid */
double calc_spindle_speed(double D_mm, double Vc_m_per_min);

/* calc_table_feed: Vf [mm/min] = fz [mm/tooth] x Z x N [RPM] */
double calc_table_feed(double fz_mm, int Z, double N_rpm);

/* calc_mrr: Q [mm3/min] = ae x ap x Vf */
double calc_mrr(double ae_mm, double ap_mm, double Vf_mm_per_min);

/* print_params: print a formatted parameter table for one condition */
void print_params (double D, double Vc, int Z, double fz,
                    double ae, double ap, const char *label);

/* --------- Main -------------------------------------------*/
int main(void) {
    printf("=== Machining Parameter Calculator ===\n");
    printf("Reference: Priest et al. 2024, Ti-6Al-4V, OSG 5-flute bull-nose\n\n");

    /*  Print table for 3 gedd rates from Priest 2024 Table 1
        Feed rates: 0.06, 0.08, 0.10 mm/tooth (Tests 1, 3, 5) */
    print_params(16.0, 90.0, 5, 0.06, 2.4, 15.0, "Test 1 (fz=0.06)");
    print_params(16.0, 90.0, 5, 0.08, 2.4, 15.0, "Test 3 (fz=0.08)");
    print_params(16.0, 90.0, 5, 0.10, 2.4, 15.0, "Test 5 (fz=0.10)");

    /* Test invalida input rejection */
    printf("--- Invalid input test ---\n");
    double result = calc_spindle_speed(-5.0, 90.0);
    printf("calc_spindle_speed(-5.0, 90.0) = %.2f (should be -1.0)\n\n", result);

    return 0;
}

/* ----- Function definitions ------------------------*/

double calc_spindle_speed(double D_mm, double Vc_m_per_min) {
    /*Input validation - physically impossible values must be rejected*/
    if (D_mm <= 0.0) {
        fprintf(stderr, "ERROR: calc_spindle_speed: D must be > 0, got %.3f\n", D_mm);
        return -1.0;
    }
    if (Vc_m_per_min <= 0.0){
        fprintf(stderr, "ERROR: calc_spindle_speed: Vc must be > 0, got %.f m/min\n", Vc_m_per_min);
        return -1.0;
    }
    /* N [RPM] = (Vc [m/min] x 1000 [mm/m]) / (pi x D [mm]) */
    return (Vc_m_per_min * 1000.0) / (M_PI * D_mm);
}

double calc_table_feed(double fz_mm, int Z, double N_rpm) {
    if(fz_mm <= 0.0) {
        fprintf(stderr, "ERROR: calc_table_feed: fz must be > 0, got %.4f mm\n", fz_mm);
        return -1.0;
    }
    if (Z < 1) {
        fprintf(stderr, "ERROR: calc_table_feed: Z must be >= 1, got %d\n", Z);
        return -1.0;
    }
    if (N_rpm <= 0.0) {
        fprintf(stderr, "ERROR: calc_table_feed: N must be > 0, got %.2f RPM\n", N_rpm);
        return -1.0;
    }
    /* Vf [mm/min] = fz [mm/tooth] x Z [teeth] x N [rev/min] */
    return fz_mm * (double)Z * N_rpm;
}

double calc_mrr(double ae_mm, double ap_mm, double Vf_mm_per_min) {
    if (ae_mm <= 0.0 || ap_mm <= 0.0 || Vf_mm_per_min <= 0.0) {
        fprintf(stderr, "ERROR: calc_mrr: all inputs must be > 0\n");
        return -1.0;
    }
    /* Q [mm3/min] = ae [mm] x ap [mm] x Vf [mm/min] */
    return ae_mm * ap_mm * Vf_mm_per_min;
}

void print_params(double D, double Vc, int Z, double fz, 
                  double ae, double ap, const char *label) {
    double N = calc_spindle_speed(D, Vc);
    double Vf = calc_table_feed(fz, Z, N);
    double Q = calc_mrr (ae, ap, Vf);

    /*  PHYSICS CHECK: tooth-passing frequency
        f_tooth [Hz] = Z x N [RPM] / 60 
        This is the frequency at which force peaks appear in the signal */
    double f_tooth_Hz = (double)Z * N /60.0;

    printf("---%s ---\n", label);
    printf("  Spindle Speed N   : %8.1f RPM\n", N);
    printf("  Table feed Vf     : %8.1f mm/min\n", Vf);
    printf("  MRR Q             : %8.1f mm3/min\n", Q);
    printf("  Tooth-pass freq   : %8.1f Hz (dynamometer must sample > %.0f Hz\n",
            f_tooth_Hz, 2.0*f_tooth_Hz);
    printf("\n");
}

