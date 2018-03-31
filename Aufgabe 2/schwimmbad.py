#!/usr/bin/python3

# 36. BWInf
# 2. Aufgabe - Schwimmbad
# Kamal Abdellatif

from itertools import product
from math import ceil
import sys

COUPON, DAYCARD, FAMILY, CHILD, TEEN, ADULT = range(6)

week_discount = .2  # 20% Wochenrabatt
coupon_discount = .1  # 10% Couponrabatt

prices = {  # Preise in Euro
	TEEN: 2.5,
	ADULT: 3.5,
	DAYCARD: 11.0,
	FAMILY: 8.0,
}

# Implementation des kartesischen Produkts von Intervallen ]0;n_i] mit Obergrenzen n_0, ..., n_k 
combinations = lambda *sizes: product( *(range(n+1) for n in sizes) )

def minimum_price(teens, adults, children, coupons, week=True, holiday=False):
	"""Gibt die Belegung fuer eine gegebene Situation aus"""
	if children and not adults:
		return None  # Keine Aufsichtspersonen

	week_f = 1 - (week_discount if week else 0)  # Wochenrabatt
	coupon_f = 1 - coupon_discount  # Couponrabatt
	people = teens + adults  # Anzahl von Personen
	# Obere Grenzen der Anzahlen von betrefflichen Personen bezgl. eines Angebots
	parameters = (
	people if coupons > 0 and not holiday else 0, # d
	min(people, coupons) if not holiday else 0,   # h
	people if week else 0,                        # t
	min(adults//2, teens//2),                     # f_22
	min(adults, teens//3),                        # f_13
	)
	min_price = float("inf")
	min_combination = None
	for discounts, freepasses, daycards, _22families, _13families in combinations(*parameters):
		if discounts > 0:
			freepasses -= 1  # Ein Coupon wird durch die ermaessigung genutzt.	

		for discount_a, freepass_a, daycard_a in combinations(discounts, freepasses, daycards):
			# _a fuer betreffliche Erwachsene
			# _t fuer betreffliche Jugendliche
			discount_t = discounts - discount_a
			freepass_t = freepasses - freepass_a
			daycard_t = daycards - daycard_a
			base_a = adults - (2*_22families+1*_13families+discount_a+freepass_a+daycard_a)
			base_t = teens  - (2*_22families+3*_13families+discount_t+freepass_t+daycard_t)

			# Testen ob die Anzahl von Personen nicht ueberschritten wird
			if base_a >= 0 and base_t >= 0:
				# Implementation der Preisfunktion
				price = base_a*prices[ADULT]*week_f + base_t*prices[TEEN]*week_f \
				      + (_22families+_13families)*prices[FAMILY] + ceil(daycards/6)*prices[DAYCARD]\
				      + (discount_a*prices[ADULT]+discount_t*prices[TEEN])*coupon_f*week_f

				if price < min_price:
					min_price = price
					min_combination = {
						"price": price,
						"discount_a": discount_a,
						"discount_t": discount_t,
						"freepass_a": freepass_a,
						"freepass_t": freepass_t,
						"daycard_a": daycard_a,
						"daycard_t": daycard_t,
						"_22families": _22families,
						"_13families": _13families,
						"base_a": base_a,
						"base_t": base_t,
						"daycards": ceil(daycards/6)
					}

	return min_combination

if __name__ == "__main__":
	# Einlesen der Datei
	with open(sys.argv[1], 'r') as fobj:
		adults = 0
		teens = 0
		children = 0

		week = eval(fobj.readline())  
		holiday = eval(fobj.readline())
		coupons = eval(fobj.readline()) # eval statt int fuer Kommentare

		for age_s in fobj.readlines():
			if not age_s:
				break
			age = eval(age_s)
			if age < 4:
				children += 1

			elif age <= 16:
				teens += 1

			else:
				adults += 1

	# Haupt-Funktionsaufruf
	solution = minimum_price(teens, adults, children, coupons, week, holiday)

	fmt_template = """
               Angebot | Erwachsene | Jugendliche
-----------------------+------------+-------------
            Basispreis | {base_a:^10d} | {base_t:^11d}
  Erm. durch Gutschein | {discount_a:^10d} | {discount_t:^11d}
       Freie Eintritte | {freepass_a:^10d} | {freepass_t:^11d}
 Erm. durch Tageskarte | {daycard_a:^10d} | {daycard_t:^11d}

                       | Anzahl
         --------------+--------
           Tageskarten | {daycards:^6d}
          2-2-Familien | {_22families:^6d}
          1-3-Familien | {_13families:^6d}

Gesamtpreis: {price:.2f}€
	"""

	# Ausgeben der Loesung
	if solution is None:
		print("Kinder bleiben unbeaufsichtigt! Kein Eintritt.")

	else:
		print(fmt_template.format(**solution))
