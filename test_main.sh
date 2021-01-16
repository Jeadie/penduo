
make clean &>  /dev/null 
make &> /dev/null

RESULT=$(./main && tail -n 1 results.csv)
if [ $? -ne 0 ]; then
  echo "Failed to run Double pendulum with initial conditions == 0.0 0.0 0.0 0.0"
  exit 1
fi

if [ $(echo $RESULT | grep '0.00000000,0.00000000,0.00000000,0.00000000' | wc -m ) -eq 0 ]; then
  echo "Double pendulum produced incorrect results."
  echo "Expected: 0.00000000,0.00000000,0.00000000,0.00000000"
  echo "Actual:   $RESULT"
  exit 1
fi

RESULT=$(./main --initial-conditions 0.0 0.0 0.78539 0.78539 && tail -n 1 results.csv)
if [ $? -ne 0 ]; then
  echo "Failed to run Double pendulum with initial conditions == 0.0 0.0 0.78539 0.78539"
  exit 1
fi

if [ $(echo $RESULT | grep '43.04222107,18.83485603,-0.20600921,-0.63068318' | wc -m) -eq 0 ]; then
  echo "Double pendulum produced incorrect results."
  echo "Expected: 43.04222107,18.83485603,-0.20600921,-0.63068318"
  echo "Actual:   $RESULT"
  exit 1
fi

echo "Tests passed"



