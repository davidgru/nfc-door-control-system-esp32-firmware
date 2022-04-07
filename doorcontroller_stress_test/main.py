import hashlib
import random
from typing import List
import requests
import json
import math


URL = "https://dc.local/token/"
PSK = "1234"

NUM_TOKENS = 200
TOKEN_LENGTH = 32
BATCH_SIZE = 50

def getRandomToken():
    token = ""
    for _ in range(TOKEN_LENGTH):
        byte = random.randint(0, 255)
        token += f'{byte:02x}'
    return token


def removeDuplicates(_list):
    _set = set(_list)
    return list(_set)


def sort(_list):
    return sorted(_list)

def makeRequestBody(tokens):
    body =  '{\n'
    body += '\t"tokens": [\n'
    for token in tokens:
        body += '\t\t{\n'
        body += f'\t\t\t"toPut": "{token}"\n'
        body += '\t\t},\n'
    if body[-2] == ',':
        body = body[:-2] + '\n'
    body += '\t]\n'
    body += '}\n'
    return body


def sendBatched(tokens: List[str]):
    while len(tokens) > 0:
        print(len(tokens))
        data = makeRequestBody(tokens[:BATCH_SIZE])
        tokens = tokens[BATCH_SIZE:]      
        resp = requests.request(
            "PUT",
            URL,
            headers={
                "psk": PSK
            },
            data=data,
            verify=False
        )
        print(f"Resp code: {resp.status_code}")
        print(f"Resp body: {resp.content}")        
    

tokensList = [getRandomToken() for _ in range(NUM_TOKENS)]
tokensUnique = removeDuplicates(tokensList)
tokensSorted = sort(tokensUnique)

tokensStr = ""
for token in tokensSorted:
    tokensStr += token

hash = hashlib.sha256(tokensStr.encode());
print(hash.hexdigest())

print(f"Unique: {len(tokensUnique)}")

sendBatched(tokensList)
        
        
resp = requests.request(
    "GET",
    URL,
    headers={
        "psk": PSK
    },
    verify=False
)

dcHash = str(resp.json()['hash'])
print(dcHash)
print(hash.hexdigest() == dcHash)