require 'yaml'

# TODO: Improve this script

def build_configurations(keys, matrix)
  conf = []
  for k in keys
    if conf.empty?
      for v in matrix[k]
        conf << ({ k => v })
      end
    else
      new_conf = []
      for v in matrix[k]
        for c in conf
          new_conf << c.merge({ k => v })
        end
      end
      conf = new_conf
    end
  end
  conf
end

def run_conf(conf, runid, exe, outdir)
  filename = [
    "g[#{conf['generations']}]",
    "d[#{conf['dna']}]",
    "m[#{conf['mutation']}]",
    "c[#{conf['crossover']}]",
    "t[#{conf['tournament']}]",
    "p[#{conf['population']}]",
    "mr[#{conf['mutation-rate']}]",
    "id[#{runid}]"
  ].join '_'
  outpath = File.join(outdir, filename + '.csv')

  params = conf.merge({ output: outpath }).to_a.map do |k,v|
    "--#{k}=#{v}"
  end.join ' '
  puts `#{exe} --csv #{params}`
end

def run_matrix(matrix)
  exe = matrix['executable']
  runs = matrix['runs']
  outdir = matrix['outdir']
  mkdir_p outdir
  params = matrix['params']
  keys = ['generations', 'dna', 'mutation', 'crossover', 'tournament', 'population', 'mutation-rate']
  conf = build_configurations keys, params
  puts "Number of executions of #{exe}: #{runs * conf.length}"
  for c in conf
    runs.times do |n|
      run_conf c, n, exe, outdir
    end
  end
end

task :matrix, [:matrix_path] do |t, args|
  matrix = YAML.load File.read args.matrix_path
  run_matrix matrix
end
