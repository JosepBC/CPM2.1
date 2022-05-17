# Commands

# Compile
```bash
make
```

Run with time
```bash
make time
```


# Zoo

## Tunel
```bash
ssh -NL 8283:zoo.lab.deim:22 <nif>@portal1-deim.urv.cat
```

## Connect
```bash
ssh -X -p 8283 <nif>@localhost
```

## Scp files
```bash
scp -P 8283 <file> <nif>@localhost:.
```

## Run script
```bash
srun -c <n_threads> -p <machine> time <file>.out
```

Where machine can be:
* teen
* orca