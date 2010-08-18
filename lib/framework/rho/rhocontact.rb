require 'bsearch'

module Rho
	class RhoContact
		class << self
			def find(param)
				pb = Phonebook::openPhonebook
				if pb.nil?
					puts "Can't open phonebook"
					return nil
				elsif param == :all or param == 'all'
					records = Phonebook::getallPhonebookRecords(pb)
					Phonebook::closePhonebook(pb)
					return records
				else
					record = Phonebook::getPhonebookRecord(pb,param)
					Phonebook::closePhonebook(pb)
					return record
				end
			end

			def create!(properties)
				pb = Phonebook::openPhonebook
				unless pb.nil?
					record = Phonebook::createRecord(pb)
					if record.nil?
						puts "Can't find record " + properties['id']
					else
						properties.each do |key,value|
							Phonebook::setRecordValue(record,key,value)
						end
						Phonebook::addRecord(pb,record)
					end
					Phonebook::closePhonebook(pb)
				end
			end

			def destroy(recordId)
				pb = Phonebook::openPhonebook
				unless pb.nil?
					record = Phonebook::openPhonebookRecord(pb,recordId)
					if record.nil?
						puts "Can't find record " + recordId
					else
						Phonebook::deleteRecord(pb,record)
					end
					Phonebook::closePhonebook(pb)
				end
			end

			def update_attributes(properties)
				pb = Phonebook::openPhonebook
				unless pb.nil?
					record = Phonebook::openPhonebookRecord(pb,properties['id'])
					if record.nil?
						puts "Can't find record " + properties['id']
					else
						properties.each do |key,value|
							Phonebook::setRecordValue(record,key,value)
						end
						Phonebook::saveRecord(pb,record)
					end
					Phonebook::closePhonebook(pb)
				end
			end

			# Examples of how to use select method:
			#
    		# selected = Rho::RhoContact.select('first_name' => 'David') { |x| x[1]['last_name']=='Taylor' }
			# ==> returns record(s) of the David Taylor
			#
    		# selected = Rho::RhoContact.select('first_name' => 'Kate')
			# ==> Returns all records of Kate
			#
    		# selected = Rho::RhoContact.select('last_name' => 'User') do |x|
    		# 	x[1]['first_name']=='Test' and x[1]['company_name']=="rhomobile"
    		# end
			# ==> returns all records of the Test User from the company rhomobile
			#
			def select(index, &block)
				key, value = index.keys[0], index.values[0]
				if @contacts.nil? or @key != key
					@key, @contacts = key, find(:all).to_a.sort! {|x,y| x[1][key] <=> y[1][key] }
				end
				found = @contacts[@contacts.bsearch_range {|x| x[1][key] <=> value}]
				unless found.nil? or block.nil?
					return found.select(&block)
				end
				return found
			end

			def select_by_name(first_last_name, &block)
				if @contacts.nil?
					@contacts = find(:all).to_a.sort! do |x,y|
						xname = (x[1]['first_name'] or "") + " " + (x[1]['last_name'] or "")
						yname = (y[1]['first_name'] or "") + " " + (y[1]['last_name'] or "")
						xname <=> yname
					end
				end

				range = @contacts.bsearch_range do |x|
					(x[1]['first_name'] or "") + " " + (x[1]['last_name'] or "") <=> first_last_name
				end
				found = @contacts[range]
				unless found.nil? or block.nil?
					return found.select(&block)
				end
				return found
			end

		end #<< self
	end # class RhoContact
end # module Rho