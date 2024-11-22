import sys
import requests
from pypinyin import lazy_pinyin

# 颜色列表
def generate_random_color(index):
    color_choices = [
        '#035785', '#DD87B2', '#BD9E60', '#91CB1A', '#892120', '#01632E',
        '#E77111', '#79949B', '#FCC602', '#AA4563', '#00A07A', '#A21777',
        '#EE0120', '#461887', '#DD0166', '#0299DC', '#88CCEE', '#CCBBCC',
        '#99D8CA', '#B6776E', '#71C9CE', '#FCE38A', '#BDD2B6', '#2C3639',
        '#DFD3C3', '#52734D', '#2D4059', '#FFD369', '#52057B', '#FFC4D0',
        '#888888', '#AAAAAA', '#444444', '#A64B2A', '#D9E4DD'
    ]
    return color_choices[index]

api = '408ba23f4126ba7b2e7b3267ae0202c9'

def save_subway_data(city_code, city_pinyin):
    url = f'https://map.amap.com/service/subway?&srhdata={city_code}_drw_{city_pinyin}.json'
    output_path = f'./data/subway_{city_pinyin}.txt'

    response = requests.get(url)
    if response.status_code != 200:
        print('response wrong when get city_pinyin')
        sys.exit(200)

    data = response.json()

    with open(output_path, 'w', encoding='utf-8') as output_file:
        # loca_code = data.get('i', [])
        # output_file.write(f"地区代码：{loca_code}\n")

        color_index = 0
        for line in data.get('l', []):
            line_index = line.get('ls')
            # 写入 id
            output_file.write(f"id: {line_index}\n")
            line_name = line.get('ln', '未知线路')
            # 写入 name
            output_file.write(f"name: {line_name}\n")
            # 写入 colour
            output_file.write(f"color: {generate_random_color(color_index)}\n")
            # 写入 fromTo
            stations = line.get('st', [])
            if stations:
                output_file.write(f"start: {stations[0].get('n', '未知站点')}\n")
                output_file.write(f"end: {stations[-1].get('n', '未知站点')}\n")
            # 写入 total
            output_file.write(f"total: {len(stations)}\n")
            # 写入站点信息
            for station in stations:
                station_index = station.get('si')
                output_file.write(f"{station_index} {station.get('n')} {station.get('sl')}\n")

            output_file.write("\n")
            color_index += 1

    print(f"./data/subway_{city_pinyin}.txt")
    sys.exit(0)


if __name__ == "__main__":
    # city_name = input("请输入城市名（如：上海、北京、广州）：")
    city_name = ""
    if len(sys.argv) > 1:
        city_name = bytes.fromhex(sys.argv[1]).decode('utf-8')
    else:
        sys.exit(100)

    url_code = f"https://restapi.amap.com/v3/config/district?keywords={city_name}&subdistrict=0&key={api}"

    response = requests.get(url_code)
    if response.status_code != 200:
        print('response wrong when get data_code')
        sys.exit(200)

    data_code = response.json()
    districts = data_code.get('districts', [])
    if not districts:
        print('districts not data')
        print(url_code)
        sys.exit(200)

    adcode = districts[0].get('adcode')

    city_code = adcode[0:4]
    city_pinyin_list = lazy_pinyin(city_name)
    city_pinyin = ''
    for py in city_pinyin_list:
        city_pinyin += py

    save_subway_data(city_code, city_pinyin)
