measured={'healthy_capacity_rps':1200,'one_instance_down_rps':750}
target_peak_rps=700
reserve_fraction=0.10
required=target_peak_rps/(1-reserve_fraction)
normal=measured['healthy_capacity_rps']>=required
degraded=measured['one_instance_down_rps']>=required
print(f'required={required:.1f}rps normal={normal} degraded={degraded}')
assert normal and not degraded
