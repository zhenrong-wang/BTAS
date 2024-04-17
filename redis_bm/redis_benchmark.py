import redis
import time  
  
r = redis.Redis(host='localhost', port=6379, db=0)  
  
r.delete('unique_numbers')  
  
print(f"Reading file ...")
with open('random.csv', 'r') as file:  
    data = [int(line.strip()) for line in file]  
file.close()

print(f"Start filtering using Set method...")
start_time = time.time()
with r.pipeline() as pipe:  
    for number in data:  
        pipe.sadd('unique_numbers', number)  
    pipe.execute()
end_time = time.time() 
print(f"Filtering done using Set method.")
elapsed_time = end_time - start_time  
print(f"{elapsed_time:.2f}sec")
unique_numbers_count = r.scard('unique_numbers')
print(f"UNIQUE_SET: {unique_numbers_count}")

print(f"Start filtering using HyperLogLog (HLL) method...")
start_time = time.time()
hyperloglog_key = 'unique_numbers_hyperloglog'
r.delete(hyperloglog_key)
r.pfadd(hyperloglog_key, *data)
end_time = time.time()
print(f"Filtering done using HLL method.") 
elapsed_time = end_time - start_time 
print(f"{elapsed_time:.2f}sec")
unique_numbers_count = r.pfcount(hyperloglog_key)
print(f"UNIQUE_HLL: {unique_numbers_count}")