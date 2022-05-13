import base64
import requests
import json

def encode_image(image):
    with open(image,"rb") as image:
        image_content = image.read()
        return base64.b64encode(image_content)

def get_label(img_path):
    encoded_img = encode_image(img_path).decode("utf-8")

    label = 'LABEL_DETECTION'
    web = 'WEB_DETECTION'
    key = 'AIzaSyB3LN7cRgrQmUz410GmYTf0HmHg8_FmUKI'
    body = {
    'requests': [
        {
            'image':{
                'content': str(encoded_img)
            },
                'features': [
                    {
                        'type': web,
                    }
                ]
            }
        ]
    }
    
    url = 'https://vision.googleapis.com/v1/images:annotate?key=' + key
    r = requests.post(url, json=body)

    j = json.loads(r.text)
    # json = {'responses': [{'webDetection': {'webEntities': [{'entityId': '/m/01jwgf', 'score': 0.40631092, 'description': 'Product'}, {'entityId': '/m/03wbdv', 'score': 0.3995, 'description': 'Jif'}, {'entityId': '/m/01qfcj', 'score': 0.3268, 'description': 'Peanut butter'}, {'entityId': '/m/02w08', 'score': 0.31167647, 'description': 'Flavor'}], 'visuallySimilarImages': [{'url': 'https://i.pinimg.com/originals/9e/66/21/9e6621a726ba3cb142eed77d9a61fe44.jpg'}, {'url': 'http://2.bp.blogspot.com/_FpnQDhYRJ3A/SGm95NwWsLI/AAAAAAAAATI/_aZL7JKEjdM/s400/DSC00912.JPG'}, {'url': 'https://s0.bukalapak.com/img/5396086283/w-1000/JIF_Peanut_Butter_Creamy_Reduced_Fat.jpg'}, {'url': 'https://www.picclickimg.com/d/l400/pict/274284672391_/VINTAGE-JIF-PEANUT-BUTTER-JAR-ORIGINAL-LID-Pet.jpg'}, {'url': 'https://didmigrate.files.wordpress.com/2010/10/img_0915.jpg'}, {'url': 'https://cdn.influenster.com/media/photo/image/54564736-0_ttQbdyU.png.355x355_q85ss0_progressive.jpg'}, {'url': 'https://www.picclickimg.com/d/l400/pict/283317719051_/VINTAGE-JIF-PEANUT-BUTTER-JAR-With-Original-Lid.jpg'}, {'url': 'https://peachesandcupcakes.files.wordpress.com/2009/09/jif.jpg?w=584'}, {'url': 'https://i.ebayimg.com/images/g/vR8AAOSwsiBeZA8H/s-l400.jpg'}, {'url': 'https://consumerist.com/consumermediallc.files.wordpress.com/2013/07/reduced_fat_jif.jpg%3Fw=1060'}], 'bestGuessLabels': [{'label': 'jif peanut butter', 'languageCode': 'en'}]}}]}

    response = j['responses']
    webDetection = response[0]['webDetection']
    similarImages = webDetection['visuallySimilarImages']
    bestGuessLabels = webDetection['bestGuessLabels']

    result = bestGuessLabels[0]["label"]
    return result


if __name__ == "__main__":
    print(get_label("test_images/camera_images/1.png"))


