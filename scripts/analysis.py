import numpy as np
from scipy.stats import chi2
from scipy import integrate
from scipy import interpolate

# ================= CONSTANTS =================

m_n = 1.6749e-27         # neutron mass in kg
g   = 9.8067             # gravity in m/s^2
q_e = 1.6022e-19         # fundamental charge in C
mg  = m_n*g/q_e*1e9/1000 # mass*gravity in neV/mm
t_b = 881.5              # neutron lifetime in seconds


# ================= GEOMETRY ================

filling_curve_length = 4069.6573961380577 # path length in x-z plane from beginning of bottle to end of source
bottle_length = 588.8                     # length of bottle
source_length = 584.5-1.5-40+75           # length in x-z plane from one end of source to cross

# ===== returns true or false if UCN position is inside storage bottle
# ===== used to remove other UCNs
def in_bottle(x,y,z):
    
    # the z planes of the bottle inside
    pos_z_end = +294.4
    neg_z_end = -294.4
    
    # the x and y planes of the bottle outside
    # big enough to get any UCN inside
    box_hx = 69.0
    box_hy = 69.0
    
    a = (x >= -box_hx) and (x <= +box_hx)
    b = (y >= -box_hy) and (x <= +box_hy)
    c = (z >= neg_z_end) and (z <= pos_z_end)
    
    inside = a and b and c
    
    return inside


# ================= GENERAL =================

# ===== number of lines in file
def file_length(file_name):
    with open(file_name) as f:
        i = -1
        for i, l in enumerate(f):
            pass
    return i+1


# ================= THEORY =================

# ===== storage time constant for cylinder radius R length L (see Golub)
def tau(E, f, V, R, L):
    
    eps = 1e-8   # a small number for integration
    n = 5000000  # a large number for integration 
    
    # velocity at height = 0 in mm/s
    v = np.sqrt(2*E*1e-9*q_e/m_n)*1000
    
    # mu average
    def mu_avg(ke):
        return 2*f*((V/ke)*np.arcsin(np.sqrt(ke/V)) - (np.sqrt(V/ke)-1))
    
    # KE as a function of height in mm
    def KE(h):
        return E - mg*h
    
    # surface perimeter in mm
    def S(h):
        return (2*L*R/np.sqrt(R**2-h**2) + 4*np.sqrt(R**2-h**2))
    
    # cross section area in mm^2
    def A(h):
        return 2*L*np.sqrt(R**2-h**2)
    
    # upper heigh bound of integrals in mm
    if E >= mg*R:
        Rstar = R 
    else:
        Rstar = E/mg
    
    # domain of integration
    x = np.linspace(-(R-eps), (Rstar-eps), n)
    
    # effective volume      
    gamma = np.trapz( A(x) * np.sqrt( (KE(x)/E) )  ,x)
    
    wall = (v/(4*gamma)) * np.trapz( (KE(x)/E) * S(x) * mu_avg(KE(x)), x)
    
    beta = 1/t_b
    
    return 1/(wall + beta)


# ================= IMPORT/EXPORT SIMULATION FILES =================

# ===== Read in alive time output file
def read_alive(aliveOutputFile):
    n_lines = file_length(aliveOutputFile)
    data = np.zeros((n_lines, 7))
    with open(aliveOutputFile, 'r') as f:
        for i, line in enumerate(f):
            line_list = line.split(" ")
            alive_time   = float(line_list[0][:-1])
            position_vector_1 = (line_list[1][1:-2]).split(",")
            position_vector_2 = (line_list[2][1:-2]).split(",")
            x1, y1, z1  = np.array(position_vector_1).astype("float")  
            x2, y2, z2  = np.array(position_vector_2).astype("float")  
            data[i,0] = alive_time
            data[i,1] = x1
            data[i,2] = y1
            data[i,3] = z1
            data[i,4] = x2
            data[i,5] = y2
            data[i,6] = z2
    
    return data

# ===== return cloud of x,y,z points at same times (snapshot)
def snapshots(snapshotInputFile, snapshotOutputFile):
    
    # Read in requested snapshot times from input file
    n_input_lines = file_length(snapshotInputFile)
    snapshot_times = np.zeros(n_input_lines-1)
    f = open(snapshotInputFile, 'r')
    snapshot_dt = float(f.readline())
    for i in range(n_input_lines-1):
        snapshot_times[i] = float(f.readline())
    f.close()

    # Read in snapshot output file
    n_output_lines = file_length(snapshotOutputFile)
    data = np.zeros((n_output_lines, 8))
    with open(snapshotOutputFile, 'r') as f:
        for i, line in enumerate(f):
            line_list = line.split(" ")
            time   = float(line_list[0][:-1])
            energy = float(line_list[1][:-1])
            position_vector = (line_list[2][1:-2]).split(",")
            velocity_vector = (line_list[3][1:-2]).split(",")
            x, y, z = np.array(position_vector).astype("float")  
            u, v, w = np.array(velocity_vector).astype("float")
            data[i,0] = time
            data[i,1] = energy
            data[i,2] = x
            data[i,3] = y
            data[i,4] = z
            data[i,5] = u
            data[i,6] = v
            data[i,7] = w

    # create snapshots
    snapshots_ke  = [] # kinetic energy snapshots
    snapshots_xyz = [] # position snapshots
    snapshots_uvw = [] # velocity snapshots
    
    # for each snapshot time...
    for i in range(len(snapshot_times)):
        
        # the current snapshot time
        t  = snapshot_times[i]
        
        # find where all these times are (within snapshot_dt) in "data"
        snapshot_indices = np.where((t <= np.array(data[:,0])) & (np.array(data[:,0])<= t + snapshot_dt))
        
        # the length of snapshot indices is how many UCNs there are at this snapshot time
        n_UCNs_at_snapshot = len(snapshot_indices[0])
        
        temp_ke  = np.zeros((n_UCNs_at_snapshot,1))
        temp_xyz = np.zeros((n_UCNs_at_snapshot,3))
        temp_uvw = np.zeros((n_UCNs_at_snapshot,3))
        for j in range(n_UCNs_at_snapshot):
            
            # go through the data and get the entries at the current snapshot time
            data_row = data[snapshot_indices[0][j],:]
            
            temp_ke[j,0] = data_row[1] # KE
            
            temp_xyz[j,0] = data_row[2] # x
            temp_xyz[j,1] = data_row[3] # y
            temp_xyz[j,2] = data_row[4] # z
            
            temp_uvw[j,0] = data_row[5] # x
            temp_uvw[j,1] = data_row[6] # y
            temp_uvw[j,2] = data_row[7] # z
            
        snapshots_ke.append(temp_ke)
        snapshots_xyz.append(temp_xyz)
        snapshots_uvw.append(temp_uvw)

    return [snapshot_times, snapshots_ke, snapshots_xyz, snapshots_uvw]



# ===== Create macro file from snapshot files at given time_index and for max_time
# ===== only include those points inside the storage bottle
def snapshot_to_macro(output_macro_filename, time_index, max_time, 
                       snapshotInputFile, snapshotOutputFile):
    
    # import the snapshots
    s = snapshots(snapshotInputFile, snapshotOutputFile)
    s_t   = s[0] # time
    s_ke  = s[1] # energy
    s_xyz = s[2] # position
    s_uvw = s[3] # velocity


    # open the macro file for outputting 
    f = open(output_macro_filename, 'w')

    # write some information
    f.write("# file name:        " + output_macro_filename + "\n")
    f.write("# time index:       " + str(time_index) + " (= " + str(s_t[time_index]) + " s)\n")
    f.write("# max time:         " + str(max_time) + " s\n")
    f.write("# snapshout input:  " + snapshotInputFile + "\n")
    f.write("# snapshout output: " + snapshotOutputFile + "\n")
    f.write("\n")
    
    # write the max simulation time
    f.write("/detector/setMaxTime_s" + " " + str(max_time) + "\n")
    f.write("/run/reinitializeGeometry" + "\n\n")

    # go through each UCN and set its energy, position, and direction then run
    n = len(s_ke[time_index])
    count = 0
    for j in range(n):
        ke = s_ke[time_index][j][0]
        x, y, z = s_xyz[time_index][j]
        u, v, w = s_uvw[time_index][j]
        # only write to file if points are inside bottle
        if in_bottle(x,y,z):
            count += 1
            f.write("#  ===== " + str(count) + " ===== #" + "\n")
            f.write("/gun/gunEnergyneV"  + " " + str(ke) + "\n")
            f.write("/gun/gunPosition"   + " " + str(x) + " " + str(y) + " " + str(z) + "\n")
            f.write("/gun/gunDirection"  + " " + str(u) + " " + str(v) + " " + str(w) + "\n")
            f.write("/run/beamOn 1" + "\n")
            f.write("\n")

    f.close()
    
# # ================= ANALYSIS =================
    
# ===== calculate histogram of data (3D points) along curve
# ===== curve file has (x,y,z) data
# ===== interpolation done with n_points
# ===== check that data are within max_distance of plane origin (to avoid counting in wrong plane due to overlaps)
def curve_hist(data, curve_file, n_points, max_distance):
    
    # load the 3d curve points
    curve = np.loadtxt(curve_file, delimiter=",")
    x_sample = curve[:,0]
    y_sample = curve[:,1]
    z_sample = curve[:,2]


    # interpolate evenly along the curve with n_points
    tck, u = interpolate.splprep([x_sample, y_sample, z_sample])
    u_eq = np.linspace(0,1,n_points)
    x_interp, y_interp, z_interp = interpolate.splev(u_eq, tck)
    
    
    # define planes normal to curve
    
    # point p0 is in plane
    p0 = np.zeros((n_points,3))
    p0[:,0] = x_interp
    p0[:,1] = y_interp
    p0[:,2] = z_interp
    
    # normal vectors to plane (last point assumes same vector as penultimate point)
    n = np.zeros((n_points,3))
    n[0:n_points-1,:] = p0[1:n_points,:] - p0[0:n_points-1,:]
    n[-1,:] = n[-2,:]
    
    # plane: a*x + b*y + c*z = d
    a = n[:,0]
    b = n[:,1]
    c = n[:,2]
    d = a*x_interp + b*y_interp + c*z_interp


    # make the histogram
    n_bins = n_points - 1
    hist = np.zeros(n_bins)
    in_pipe_x = []
    in_pipe_y = []
    in_pipe_z = []
    # loop through each data point
    for point in data:
        # loop through each bin
        for k in range(n_points-1):
            # is it above the plane?
            above_this_plane = ((a[k]*point[0] + b[k]*point[1] + c[k]*point[2]) > d[k])
            # is it below the next plane?
            below_next_plane = ((a[k+1]*point[0] + b[k+1]*point[1] + c[k+1]*point[2]) < d[k+1])
            # how far away is it from the origin of the plane?
            distance = np.sqrt((point[0]-x_interp[k])**2+(point[1]-y_interp[k])**2+(point[2]-z_interp[k])**2)
            # is it closer than max_distance?
            within_distance = (distance < max_distance)
            # if yes to all the above, then record it in the histogram
            if (above_this_plane and below_next_plane and within_distance):
                in_pipe_x.append(point[0])
                in_pipe_y.append(point[1])
                in_pipe_z.append(point[2])
                hist[k] += 1
                break

    in_pipe = np.zeros((len(in_pipe_x),3))
    in_pipe[:,0] = in_pipe_x
    in_pipe[:,1] = in_pipe_y
    in_pipe[:,2] = in_pipe_z

    # the bin centers
    x_bin = (u_eq[1:] - u_eq[:-1])/2 + u_eq[:-1]
    
    return np.array([x_bin, hist])




# ===== class for doing Lilliefors test to check for exponential distribution
# ===== also return some other things like T0 estimate and confidence interval
class Lilliefors:
    def __init__(self, X):
        self.X = X
        self.X.sort()
        self.N = len(X)     
        self.S = np.arange(1.0, self.N+1.0)/self.N
        self.Fhat = 1 - np.exp(-self.X/np.mean(self.X))
        self.D = np.max(np.absolute(self.Fhat-self.S))
        
        # valid of N > 30, from paper
        self.alpha1  = 1.25/np.sqrt(self.N)
        self.alpha5  = 1.06/np.sqrt(self.N)
        self.alpha10 = 0.96/np.sqrt(self.N)
        
        # MLE of T0
        self.T0 = np.mean(self.X)
        
        # cdf
        self.cdf = "unset"
        self.pD =  "unset"
        
    # confidence interval on T0
    def ConfidenceInterval(self, alpha):
        LB = chi2.ppf(0.5*alpha,     2*int(self.N))
        UB = chi2.ppf(1.0-0.5*alpha, 2*int(self.N))
        intervalBot = 2*self.N*self.T0/UB
        intervalTop = 2*self.N*self.T0/LB
        return np.array([intervalBot, intervalTop])
    
    # monte carlo calculation of D value probability
    def DProbability(self):
        n_samples = self.N
        S = np.arange(1.0, n_samples+1.0)/n_samples

        n_runs = 200000
        D = np.zeros(n_runs)
        for i in range(n_runs):
            X = np.random.exponential(scale=1.0, size=n_samples)
            X.sort()
            Fhat = 1 - np.exp(-X/np.mean(X))
            D[i] = np.max(np.absolute(Fhat-S))
            
        nbins=100
        bin_edges   = np.linspace(0.0, 1.1*np.max(D), nbins+1)
        bin_centers = (bin_edges[1:]+bin_edges[:-1])/2
        counts = np.histogram(D, bin_edges)[0]
        
        cdf = integrate.cumtrapz(counts, bin_centers, initial=0)
        cdf/= cdf[-1]
        
        self.cdf = cdf
        self.pD = 1.0 - np.interp(self.D, bin_centers, cdf)
        return self.pD